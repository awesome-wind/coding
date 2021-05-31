package main

import "fmt"

type TreeNode struct {
	Val   int
	Left  *TreeNode
	Right *TreeNode
}

func main() {
	arr := []int{1, 2, 3, 4, 5, 6, 7}
	tree := createBinaryTree(arr, 0)
	result := printTreeByLevel(tree)
	fmt.Println(result)
}

func createBinaryTree(arr []int, index int) *TreeNode {
	if index >= len(arr) {
		return nil
	}
	node := &TreeNode{arr[index], nil, nil}

	node.Left = createBinaryTree(arr, 2*index+1)
	node.Right = createBinaryTree(arr, 2*index+2)

	return node
}

func printTreeByLevel(root *TreeNode) [][]int {
	if root == nil {
		return nil
	}
	slice1 := make([]*TreeNode, 0)
	slice2 := make([]*TreeNode, 0)
	result := make([][]int, 0)

	slice1 = append(slice1, root)
	for len(slice1) > 0 {
		subResult := make([]int, 0)
		for i := 0; i < len(slice1); i++ {
			node := slice1[i]
			subResult = append(subResult, node.Val)

			if node.Left != nil {
				slice2 = append(slice2, node.Left)
			}
			if node.Right != nil {
				slice2 = append(slice2, node.Right)
			}
		}
		result = append(result, subResult)

		slice1 = nil
		slice1 = append(slice1, slice2...)
		slice2 = nil
	}
	return result
}
