#include "KMR.h"

KMR::frag::frag(int a, int b, const std::vector<int> &mx, const std::vector<std::vector<int>> &k)
    : l(a), r(b), mx_len_kmr(mx), kmr(k) {}

bool KMR::frag::operator<(const frag &y) const {
    int len1 = r - l + 1;
    int len2 = y.r - y.l + 1;
    int minLen = std::min(len1, len2);
    int k = mx_len_kmr[minLen];
    int idx = std::min(r - std::max(l + (1 << k) - 1, r - (1 << k)),
                       y.r - std::max(y.l + (1 << k) - 1, y.r - (1 << k)));
    std::pair<int, int> rank1 = {kmr[r][k], kmr[r - idx][k]};
    std::pair<int, int> rank2 = {kmr[y.r][k], kmr[y.r - idx][k]};
    if (rank1 != rank2) return rank1 < rank2;
    return len1 < len2;
}

void KMR::buildReversekmr(const std::string &s) {
    int n = (int)s.size();
    int logN = 0;
    while ((1 << logN) <= n) logN++;
    kmr.assign(n, std::vector<int>(logN, 0));
    std::vector<std::pair<std::pair<int, int>, int>> suffixes(n);
    for (int i = 0; i < n; i++) {
        kmr[i][0] = s[i];
    }
    for (int k = 1; k < logN; k++) {
        int len = 1 << (k - 1);
        for (int i = 0; i < n; i++) {
            if (i - len >= 0) {
                suffixes[i] = {{kmr[i][k - 1], kmr[i - len][k - 1]}, i};
            }
            else {
                suffixes[i] = {{kmr[i][k - 1], -1}, i};
            }
        }
        std::sort(suffixes.begin(), suffixes.end());
        int cr = 0;
        for (int i = 0; i < n; i++) {
            if (i > 0 && suffixes[i].first != suffixes[i - 1].first) {
                cr++;
            }
            kmr[suffixes[i].second][k] = cr;
        }
    }
}

KMR::KMR(const std::string &s) {
    int n = (int)s.size();
    mx_len_kmr.assign(n + 1, 0);
    int pm = 0;
    for (int i = 0; i <= n; i++) {
        if ((1 << (pm + 1)) <= i) pm++;
        mx_len_kmr[i] = pm;
    }
    buildReversekmr(s);
}

void KMR::addprefix(const int &r) {
    searcher.insert(frag(0, r, mx_len_kmr, kmr));
}

std::pair<int, int> KMR::lowerBound(const int &l, const int &r) {
    auto it = searcher.lower_bound(frag(l, r, mx_len_kmr, kmr));
    std::pair<int, int> res = {-1, -1};
    if (it != searcher.end()) res = {(*it).l, (*it).r};
    return res;
}

bool KMR::fit(const int &a1, const int &a2, const int &b1, const int &b2) {
    if (a2 - a1 > b2 - b1) return false;
    int a = a1, b = a2;
    int c = b2 - b + a, d = b2;
    int len1 = b - a + 1;
    int len2 = d - c + 1;
    if (len1 != len2) {
        return false;
    }
    int k = mx_len_kmr[len1];
    std::pair<int, int> rank1 = {kmr[b][k], kmr[a + (1 << k) - 1][k]};
    std::pair<int, int> rank2 = {kmr[d][k], kmr[c + (1 << k) - 1][k]};
    return rank1 == rank2;
}
