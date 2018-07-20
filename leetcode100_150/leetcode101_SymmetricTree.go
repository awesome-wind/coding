package main

import "fmt"

type TreeNode struct {
	Val   int
	Left  *TreeNode
	Right *TreeNode
}

func main() {
	arr := []int{1, 2, 2, 3, 4, 4, 3}
	tree := createTree(arr, 0)
	printTreeByLevel(tree)
	fmt.Printf("是否是镜像的：%v", isSymmetric(tree))
}

func createTree(arr []int, index int) *TreeNode {
	if index >= len(arr) {
		return nil
	}
	tree := &TreeNode{arr[index], nil, nil}
	tree.Left = createTree(arr, 2*index+1)
	tree.Right = createTree(arr, 2*index+2)

	return tree
}

func printTreeByLevel(tree *TreeNode) {
	slice1 := make([]*TreeNode, 0)
	slice2 := make([]*TreeNode, 0)

	if tree == nil {
		return
	}
	slice1 = append(slice1, tree)
	for len(slice1) != 0 {
		for _, node := range slice1 {
			fmt.Printf("%d ", node.Val)

			if node.Left != nil {
				slice2 = append(slice2, node.Left)
			}
			if node.Right != nil {
				slice2 = append(slice2, node.Right)
			}
		}
		fmt.Println()

		slice1 = slice1[:0]
		slice1 = append(slice1, slice2...)
		slice2 = slice2[:0]

	}
}

func isSymmetric(root *TreeNode) bool {
	return root == nil || symmetric(root.Left, root.Right)
}

func symmetric(left *TreeNode, right *TreeNode) bool {
	if left == nil || right == nil {
		return left == right
	}

	if left.Val != right.Val {
		return false
	}

	return symmetric(left.Left, right.Right) || symmetric(left.Right, right.Left)
}
