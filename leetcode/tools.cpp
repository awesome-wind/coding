#include "leetcode/tools.h"

#include <iostream>

void show_list(ListNode* list) {
    if (list == nullptr) {
        return;
    }

    ListNode* head = list;

    std::cout << head->val;
    while (head->next) {
        head = head->next;
        std::cout << " -> " << head->val;
    }
    std::cout << std::endl;
}

void show_two_dimensional_vector(std::string str, std::vector<std::vector<int>>& res) {
    std::cout << "print:" << str << std::endl;
    for (auto& tmp : res) {
        std::cout << "[";
        int i = 0;
        for (i = 0; i < tmp.size() - 1; i++) {
            std::cout << tmp[i] << ",";
        }
        std::cout << tmp[i] << "]" << std::endl;
    }
}