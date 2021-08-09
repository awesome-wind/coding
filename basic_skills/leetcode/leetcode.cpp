#include "leetcode/leetcode.h"

#include <iostream>

bool LeetCode::is_match(std::string& s, std::string& p) {
    while (s.size() && p.size() && p.back() != '*') {
        if (s.back() == p.back() || p.back() == '?') {
            s.pop_back();
            p.pop_back();
        } else {
            return false;
        }
    }

    if (p.empty()) {
        return s.empty();
    }

    std::cout << "s:" << s << " p:" << p << std::endl;

    int i = 0, j = 0, ie = -1, je = -1;
    while (i < s.size() && j < p.size()) {
        if (p[j] == '*') {
            j++;
            ie = i;
            je = j;
        } else if (s[i] == p[j] || p[j] == '?') {
            i++;
            j++;
        } else if (je != -1 && je + 1 < p.size()) {
            ie++;
            i = ie;
            j = je;
        } else {
            return false;
        }
    }

    while (j < p.size()) {
        if (p[j] != '*') {
            return false;
        }
        j++;
    }
    return true;
}