package main

import "fmt"

func main() {
	S := "rabbbit"
	T := "rabbit"

	num := numDistinct(S, T)
	fmt.Println("命中的子串的个数为：", num)
}

//动态规划求解，当遍历到dp[i][j]时
//如果t[i-1] == s[j-1] 那么dp[i][j] = dp[i][j-1] + dp[i-1][j-1]
//否则dp[i][j]=dp[i][j-1]
func numDistinct(s string, t string) int {
	lenS := len(s) + 1
	lenT := len(t) + 1
	dp := make([][]int, 0)

	for i := 0; i < lenT; i++ {
		tmp := make([]int, lenS)
		dp = append(dp, tmp)
	}

	for i := 0; i < lenT; i++ {
		dp[i][0] = 0
	}
	for j := 0; j < lenS; j++ {
		dp[0][j] = 1
	}

	for i := 1; i < lenT; i++ {
		for j := 1; j < lenS; j++ {
			if t[i-1] == s[j-1] {
				dp[i][j] = dp[i][j-1] + dp[i-1][j-1]
			} else {
				dp[i][j] = dp[i][j-1]
			}
		}
	}
	return dp[len(t)][len(s)]
}
