#include "leetcode/leetcode.h"

#include <memory>

#include "gtest/gtest.h"
#include "leetcode/tools.h"

TEST(LeetCodeTest, wildcard_test) {
    std::unique_ptr<LeetCode> code = std::make_unique<LeetCode>();

    std::string s = "abcdefgh";
    std::string p = "ab?c*f?h";
    EXPECT_FALSE(code->is_match(s, p));

    s = "abcdefgh";
    p = "a*m*gh";
    EXPECT_FALSE(code->is_match(s, p));

    s = "abcdefgh";
    p = "a?c*?h";
    EXPECT_TRUE(code->is_match(s, p));
}

TEST(LeetCodeTest, merge_k_sorted_lists) {
    std::vector<ListNode*> lists(3);
    ListNode *head = new ListNode(-1), *head1 = new ListNode(-1), *head2 = new ListNode(-1);
    lists[0] = head;
    lists[1] = head1;
    lists[2] = head2;
    for (int i = 0; i < lists.size(); ++i) {
        head->next = new ListNode(3 * i);
        head = head->next;
        head1->next = new ListNode(3 * i + 1);
        head1 = head1->next;
        head2->next = new ListNode(3 * i + 2);
        head2 = head2->next;
    }

    for (int i = 0; i < lists.size(); ++i) {
        lists[i] = lists[i]->next;
    }

    ListNode* ans_head = new ListNode(-1);
    ListNode* ans_list = ans_head;
    for (int i = 0; i < lists.size() * 3; ++i) {
        ans_head->next = new ListNode(i);
        ans_head = ans_head->next;
    }
    ans_list = ans_list->next;
    // show_list(ans_list);

    // for (int i = 0; i < lists.size(); ++i) {
    //     show_list(lists[i]);
    // }

    std::unique_ptr<LeetCode> code = std::make_unique<LeetCode>();
    // ListNode* res = code->merge_lists(lists, 0);
    // ListNode* ans = ans_list;
    // while (res || ans) {
    //     EXPECT_EQ(res->val, ans->val);
    //     res = res->next;
    //     ans = ans->next;
    // }

    // std::cout << "mode 0 merged lists:" << std::endl;
    // for (int i = 0; i < lists.size(); ++i) {
    //     show_list(lists[i]);
    // }

    ListNode* res1 = code->merge_lists(lists, 1);
    ListNode* ans1 = ans_list;
    while (res1 || ans1) {
        EXPECT_EQ(res1->val, ans1->val);
        res1 = res1->next;
        ans1 = ans1->next;
    }

    std::cout << "mode 1 merged lists:" << std::endl;
    for (int i = 0; i < lists.size(); ++i) {
        show_list(lists[i]);
    }

    // ListNode* res2 = code->merge_lists(lists, 2);
    // ListNode* ans2 = ans_list;
    // show_list(res2);
    // show_list(ans2);
    // while (res2 || ans2) {
    //     EXPECT_EQ(res2->val, ans2->val);
    //     res2 = res2->next;
    //     ans2 = ans2->next;
    // }
}

TEST(LeetCodeTest, permute_test) {
    std::unique_ptr<LeetCode> code = std::make_unique<LeetCode>();
    std::vector<int> nums = {1, 2, 3};
    auto res = code->permute(nums, false);
    show_two_dimensional_vector("simple permute", res);

    int res_size = 1;
    for (int i = 1; i <= nums.size(); i++) {
        res_size *= i;
    }

    EXPECT_EQ(res.size(), res_size);

    nums = {1, 1, 2};
    res = code->permute(nums, true);
    show_two_dimensional_vector("permute with repeated num", res);
    EXPECT_EQ(res.size(), 3);
}