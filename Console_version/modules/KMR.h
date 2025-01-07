#ifndef KMR_H
#define KMR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <utility>

class KMR {
private:
    struct frag {
        int l, r;
        const std::vector<int> &mx_len_kmr;
        const std::vector<std::vector<int>> &kmr;

        frag(int a, int b, const std::vector<int> &mx, const std::vector<std::vector<int>> &k);

        bool operator<(const frag &y) const;
    };

    std::vector<int> mx_len_kmr;
    std::vector<std::vector<int>> kmr;
    std::set<frag> searcher;

    void buildReversekmr(const std::string &s);

public:
    KMR(const std::string &s);

    void addprefix(const int &r);

    std::pair<int, int> lowerBound(const int &l, const int &r);

    bool fit(const int &a1, const int &a2, const int &b1, const int &b2);
};

#endif // KMR_H
