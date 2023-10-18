package main

import "fmt"

func quickSort(nums []int, star, end int) {
	if star > end {
		return
	}

	i, j := star, end
	key := nums[star]

	for i < j {
		for i < j && nums[j] > key {
			j--
		}
		for i < j && nums[i] <= key {
			i++
		}

		if i < j {
			nums[i], nums[j] = nums[j], nums[i]
		}
	}
	nums[star], nums[i] = nums[i], key
	quickSort(nums, star, i-1)
	quickSort(nums, i+1, end)
}

func main() {
	nums := []int{2, 5, 6, 1, 8, 9}

	quickSort(nums, 0, len(nums)-1)

	for _, i := range nums {
		fmt.Println(i)
	}
}
