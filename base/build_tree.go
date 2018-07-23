package base

import "fmt"

type TreeNode struct {
	Val   int
	Left  *TreeNode
	Right *TreeNode
}

func main() {
	arr := []int{1, 2, 3, 4, 5, 6, 7, 8}
	tree := createTree(arr, 0)
	ret := printTree(tree)
	fmt.Println(ret)
}

func printTree(node *TreeNode) (ret string) {
	if node == nil {
		return "#!"
	}
	ret += fmt.Sprintf("%d!", node.Val)
	ret += printTree(node.Left)
	ret += printTree(node.Right)
	return ret
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
