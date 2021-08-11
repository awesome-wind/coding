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