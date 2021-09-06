#include "leetcode/leetcode.h"
#include "leetcode/tools.h"

void LeetCode::rotate(std::vector<std::vector<int>>& image, int mode) {
    if (image.empty()) {
        return;
    }

    int tmp = -1, k = image.size() - 1;
    if (mode == 1) {
        for (int i = 0; i < (k + 1) / 2; i++) {
            for (int j = 0; j < k / 2 + 1; j++) {
                int tmp = image[i][j];
                image[i][j] = image[k - j][i];
                image[k - j][i] = image[k - i][k - j];
                image[k - i][k - j] = image[j][k - i];
                image[j][k - i] = tmp;
            }
        }
    } else if (mode == 2) {
        for (int i = 0; i < (k + 1) / 2; i++) {
            for (int j = 0; j <= k; j++) {
                std::swap(image[i][j], image[k - i][j]);
            }
        }

        for (int i = 0; i <= k; i++) {
            for (int j = i; j <= k; j++) {
                std::swap(image[i][j], image[j][i]);
            }
        }
    }
}