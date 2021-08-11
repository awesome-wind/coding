#include "leetcode/leetcode.h"
#include "leetcode/tools.h"

ListNode* merge_2_list(ListNode* l1, ListNode* l2) {
    if ((!l1) || (!l2))
        return l1 ? l1 : l2;

    ListNode node;
    ListNode* head = &node;
    ListNode *head1 = l1, *head2 = l2;

    while (head1 && head2) {
        if (head1->val <= head2->val) {
            head->next = head1;
            head1 = head1->next;
        } else {
            head->next = head2;
            head2 = head2->next;
        }
        head = head->next;
    }
    head->next = head1 ? head1 : head2;
    return node.next;
}

ListNode* sequence_merge_lists(std::vector<ListNode*>& lists) {
    ListNode* ans = nullptr;
    for (auto list : lists) {
        ans = merge_2_list(ans, list);
    }
    return ans;
}

ListNode* devide_conquer(std::vector<ListNode*>& lists, int left, int right) {
    if (left > right) {
        return nullptr;
    }
    if (left == right) {
        return lists[left];
    }
    int mid = left + (right - left) / 2;
    ListNode* left_node = devide_conquer(lists, left, mid);
    ListNode* right_node = devide_conquer(lists, mid + 1, right);
    return merge_2_list(left_node, right_node);
}

ListNode* devide_conquer_merge_lists(std::vector<ListNode*>& lists) {
    return devide_conquer(lists, 0, (int)lists.size() - 1);
}

ListNode* improve_merge_lists(std::vector<ListNode*>& lists) {
    int left = 0, right = (int)lists.size() - 1;
    while (left < right) {
        if (right == 0) {
            return lists[0];
        }

        if (left < right) {
            lists[left] = merge_2_list(lists[left], lists[right]);
            left++;
            right--;
        }
        if (left == right || left == right + 1) {
            left = 0;
        }
    }
    return lists[0];
}

ListNode* LeetCode::merge_lists(std::vector<ListNode*>& lists, int mode) {
    if (lists.empty()) {
        return nullptr;
    }
    if (lists.size() == 1) {
        return lists[0];
    }

    switch (mode) {
        case 0:
            return sequence_merge_lists(lists);
        case 1:
            return devide_conquer_merge_lists(lists);
        case 2:
            return improve_merge_lists(lists);
        default:
            return nullptr;
    };
}