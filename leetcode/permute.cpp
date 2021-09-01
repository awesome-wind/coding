#include <iostream>

#include "leetcode/leetcode.h"
#include "leetcode/tools.h"

void backtrack(std::vector<std::vector<int>>& res, std::vector<int>& tmp, int index, int end) {
    if (index == end) {
        // show_vector("result", tmp);
        res.emplace_back(tmp);
        return;
    }

    for (int i = index; i < end; ++i) {
        std::swap(tmp[i], tmp[index]);
        // std::cout << "pre i:" << tmp[i] << " index:" << tmp[index] << std::endl;
        // show_vector("pre", tmp);
        backtrack(res, tmp, index + 1, end);
        std::swap(tmp[i], tmp[index]);
        // std::cout << "back i:" << tmp[i] << " index:" << tmp[index] << std::endl;
        // show_vector("back", tmp);
    }
}

void backtrack_repeat(std::vector<std::vector<int>>& res, std::vector<int>& nums, std::vector<int>& tmp,
                      std::vector<bool>& visit, int index) {
    if (index == nums.size()) {
        res.push_back(tmp);
        return;
    }

    for (int i = 0; i < nums.size(); i++) {
        if (visit[i] || (i > 0 && nums[i] == nums[i - 1] && !visit[i - 1])) {
            continue;
        }

        tmp.push_back(nums[i]);
        visit[i] = true;
        // show_vector<bool>("pre bool", visit);
        // show_vector<int>("pre int", tmp);
        backtrack_repeat(res, nums, tmp, visit, index + 1);
        visit[i] = false;
        tmp.pop_back();
        // show_vector<bool>("after bool", visit);
        // show_vector<int>("after int", tmp);
    }
}

std::vector<std::vector<int>> LeetCode::permute(std::vector<int> nums, bool repeat) {
    std::vector<std::vector<int>> res;
    if (repeat) {
        std::sort(nums.begin(), nums.end());
        std::vector<bool> visit(nums.size(), false);
        std::vector<int> tmp;
        backtrack_repeat(res, nums, tmp, visit, 0);
    } else {
        backtrack(res, nums, 0, (int)nums.size());
    }

    return res;
}
