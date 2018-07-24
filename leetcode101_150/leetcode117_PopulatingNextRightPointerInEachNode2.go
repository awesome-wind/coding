package main

import (
	"fmt"

	"../base"
	"../model"
)

func main() {
	arr := []int{1, 2, 3, 4, 5, -1, 7}
	tree := base.CreateBinaryTree(arr, 0)
	base.PrintTreeByLevel(tree)
	root := getNextNode(tree)
	printLinkTreeByLevel(root)
}

func getNextNode(root *model.TreeLinkNode) *model.TreeLinkNode {
	res_root := root
	head := &model.TreeLinkNode{0, nil, nil, nil}
	step := head

	for root != nil {
		if root.Left != nil && root.Left.Val != -1 {
			step.Next = root.Left
			step = step.Next
		}
		if root.Right != nil && root.Right.Val != -1 {
			step.Next = root.Right
			step = step.Next
		}
		root = root.Next
		if root == nil {
			step = head
			root = head.Next
			step.Next = nil
		}
	}
	fmt.Printf("返回的顶点的值为%d\n", res_root.Val)
	return res_root
}

func printLinkTreeByLevel(root *model.TreeLinkNode) {
	if root == nil {
		return
	}
	slice1 := make([]*model.TreeLinkNode, 0)
	slice2 := make([]*model.TreeLinkNode, 0)

	slice1 = append(slice1, root)
	for len(slice1) > 0 {
		for i := 0; i < len(slice1); i++ {
			node := slice1[i]
			if node.Val != -1 {
				fmt.Printf("%d", node.Val)
			}
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
