package main

import (
	"fmt"
	"math"
)

func main() {
	sli1 := []int{1, 3, 5, 7, 9, 11}
	sli2 := []int{2, 4, 6, 8, 10}

	fmt.Println("中位数为：%v\n", findMedianSortedArrays(sli1, sli2))
}

func findMedianSortedArrays(nums1 []int, nums2 []int) float64 {
	total := (len(nums1) + len(nums2))

	if total%2 == 1 {
		return findKth(nums1, nums2, total/2+1)
	} else {
		return (findKth(nums1, nums2, total/2) + findKth(nums1, nums2, total/2+1)) / 2
	}

}

func findKth(nums1 []int, nums2 []int, k int) float64 {
	if len(nums1) > len(nums2) {
		return findKth(nums2, nums1, k)
	}
	if len(nums1) == 1 {
		return float64(nums2[k-1])
	}

	if k == 1 {
		return math.Min(float64(nums1[0]), float64(nums2[0]))
	}

	pa := int(math.Min(float64(k/2), float64(len(nums1))))
	pb := k - pa
	if nums1[pa-1] < nums2[pb-1] {
		return findKth(nums1[pa:], nums2, k-pa)
	} else if nums1[pa-1] > nums2[pb-1] {
		return findKth(nums1, nums2[pb:], k-pb)
	} else {
		return float64(nums1[pa-1])
	}
}
