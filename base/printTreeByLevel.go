package base

import "fmt"
import "../model"

func PrintTreeByLevel(root *model.TreeLinkNode) {
	if root == nil {
		return
	}
	slice1 := make([]*model.TreeLinkNode, 0)
	slice2 := make([]*model.TreeLinkNode, 0)

	slice1 = append(slice1, root)
	for len(slice1) > 0 {
		for i := 0; i < len(slice1); i++ {
			node := slice1[i]
			fmt.Printf("%d ", node.Val)

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

func CreateBinaryTree(arr []int, index int) *model.TreeLinkNode {
	if index >= len(arr) {
		return nil
	}
	node := &model.TreeLinkNode{arr[index], nil, nil, nil}
	node.Left = CreateBinaryTree(arr, 2*index+1)
	node.Right = CreateBinaryTree(arr, 2*index+2)

	return node
}
