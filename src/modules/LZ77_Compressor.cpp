#include "LZ77_Compressor.h"
#include "KMR.h"
void LZ77_COMPRESSOR::writeInt(const unsigned int x, std::ofstream &pf) {
    unsigned char xp[3];
    xp[0] = (x & 0xFF);
    xp[1] = ((x >> 8) & 0xFF);
    xp[2] = ((x >> 16) & 0xFF);
    for (int i = 0; i < 3; i++) {
        pf.put(xp[i]);
    }
}

std::vector<std::array<int, 3>> LZ77_COMPRESSOR::compress_block(const std::string &s) {
    int n = s.size();
    KMR kmr(s);
    std::vector<std::array<int, 3>> res;
    for (int i = 0; i < n; i++) {
        int wsk = i;
        std::array<int, 3> crypt = {0, 0, s[i]};
        while (wsk < n) {
            std::pair<int, int> try_find = kmr.lowerBound(i, wsk);
            if (try_find.first != -1) {
                if (kmr.fit(i, wsk, try_find.first, try_find.second)) {
                    wsk++;
                    crypt[0] = wsk - try_find.second - 1;
                    crypt[1] = wsk - i;
                    continue;
                }
            }
            break;
        }
        crypt[2] = (wsk == n) ? '~' : s[wsk];
        for (int j = i; j <= std::min(wsk, n - 1); j++) {
            kmr.addprefix(j);
        }
        res.push_back(crypt);
        i = wsk;
    }
    return res;
}

LZ77_COMPRESSOR::LZ77_COMPRESSOR() {}

int LZ77_COMPRESSOR::compress(const std::string FileIN, const std::string FileOUT) {
    std::ifstream IN(FileIN, std::ios::binary);
    if (!IN.is_open()) {
        return 1;
    }

    std::ofstream OUT(FileOUT, std::ios::binary);
    if (!OUT.is_open()) {
        return 2;
    }

    writeInt(START_OF_FILE, OUT);
    writeInt(BLOCK_TERMINATOR, OUT);
    writeInt(FILE_TERMINATOR, OUT);

    std::string s = "";
    int block_number = 0;

    std::vector<std::pair<int, std::vector<std::array<int, 3>>>> res;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::pair<int, std::string>> task_queue;
    std::atomic<bool> finished(false);
    std::vector<std::thread> workers;
    const int num_threads = std::max(1, static_cast<int>(std::thread::hardware_concurrency())-1);

    auto worker = [this, &mtx, &cv, &task_queue, &res, &finished]() {
        while (true) {
            std::pair<int, std::string> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [&]() { return !task_queue.empty() || finished; });
                if (task_queue.empty() && finished) break;
                task = task_queue.front();
                task_queue.pop();
            }

            auto result = compress_block(task.second);
            {
                std::lock_guard<std::mutex> lock(mtx);
                res.push_back({task.first, result});
            }
        }
    };

    for (int i = 0; i < num_threads; i++) {
        workers.emplace_back(worker);
    }

    char ch;
    while (IN.get(ch)) {
        s += ch;
        if ((int)s.size() == BLOCK_SIZE) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                task_queue.push({block_number++, s});
            }
            cv.notify_one();
            s = "";
        }
    }

    if (!s.empty()) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            task_queue.push({block_number++, s});
        }
        cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all();

    for (auto &t : workers) {
        if (t.joinable()) t.join();
    }

    std::vector<int> srt(block_number, 0);
    for (int i = 0; i < block_number; i++) {
        srt[res[i].first] = i;
    }
    for (int i = 0; i < block_number; i++) {
        std::pair<int, std::vector<std::array<int, 3>>> &v = res[srt[i]];
        for (auto &curr : v.second) {
            writeInt(curr[0], OUT);
            writeInt(curr[1], OUT);
            OUT.put(curr[2]);
        }
        writeInt(BLOCK_TERMINATOR, OUT);
    }
    writeInt(FILE_TERMINATOR, OUT);
    IN.close();
    OUT.close();
    return 0;
}

