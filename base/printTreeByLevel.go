package base

import "fmt"
import "../model"

func PrintTreeByLevel(root *model.TreeNode) {
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
