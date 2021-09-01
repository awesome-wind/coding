#pragma once

#include <leetcode/leetcode.h>

#include <iostream>

void show_list(ListNode* list);

template <typename T>
void show_vector(std::string str, std::vector<T>& tmp) {
    std::cout << str << " vector:";
    for (T num : tmp) {
        std::cout << " " << num;
    }
    std::cout << std::endl;
}

void show_two_dimensional_vector(std::string str, std::vector<std::vector<int>>& res);