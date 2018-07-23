package main

import "fmt"
import "../model"
import "../base"

func main() {
	arr := []int{1, 2, 3, 4, 5, 6, 7}
	tree := createBinaryTree(arr, 0)
	base.PrintTreeByLevel(tree)
	populateNext(tree)
	printTree(tree)
}

func createBinaryTree(arr []int, index int) *model.TreeNode {
	if index >= len(arr) {
		return nil
	}
	node := &model.TreeNode{arr[index], nil, nil, nil}
	node.Left = createBinaryTree(arr, 2*index+1)
	node.Right = createBinaryTree(arr, 2*index+2)

	return node
}

func printTree(root *model.TreeNode) {
	if root == nil {
		return
	}
	slice1 := make([]*model.TreeNode, 0)
	slice2 := make([]*model.TreeNode, 0)

	slice1 = append(slice1, root)
	for len(slice1) > 0 {
		for i := 0; i < len(slice1); i++ {
			node := slice1[i]
			fmt.Printf("%d ", node.Val)
			if node.Next != nil {
				fmt.Print("->")
			}
			if node.Left != nil {
				slice2 = append(slice2, node.Left)
			}
			if node.Right != nil {
				slice2 = append(slice2, node.Right)
			}
		}
		fmt.Println()

		slice1 = nil
		slice1 = append(slice1, slice2...)
		slice2 = nil
	}
}

func populateNext(root *model.TreeNode) {
	if root == nil {
		return
	}

	p := root.Next
	if root.Left != nil {
		root.Left.Next = root.Right
		if p != nil {
			root.Right.Next = p.Left
		}
	}

	populateNext(root.Left)
	populateNext(root.Right)

	fmt.Printf("现在的顶点的值为：%d\n", root.Val)
}
