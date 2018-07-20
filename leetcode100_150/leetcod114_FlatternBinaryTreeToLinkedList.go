package main

import "fmt"
import "strconv"

type TreeNode struct {
	Val   int
	Left  *TreeNode
	Right *TreeNode
}

func main() {
	arr := []int{1, 2, 3, 4, 5, 6, 7, 8}
	tree := createTree(arr, 0)
	tree = flatten(tree)
	printListTree(tree)
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

/*
	1、将temp节点移动到根节点的左子树节点，延展到该左子树右叶节点
	2、将根节点的右子树接到该当前的temp节点，将根节点的左子树移动为右子树，并将做字数置空
	3、根节点向右移动一个节点，遍历完成
	https://segmentfault.com/a/1190000010503483
*/
func flatten(root *TreeNode) *TreeNode {
	if root == nil {
		return nil
	}
	current := root
	temp := root
	for current != nil {
		if current.Left != nil {
			temp = current.Left
			for temp.Right != nil {
				temp = temp.Right
			}
			temp.Right = current.Right
			current.Right = current.Left
			current.Left = nil
		}
		current = current.Right
	}

	return root
}

func printListTree(node *TreeNode) {
	if node == nil {
		return
	}
	if node.Left != nil {
		fmt.Println("Error,the code is wrong")
	}
	fmt.Println(strconv.Itoa(node.Val) + " ")
	printListTree(node.Right)
}
