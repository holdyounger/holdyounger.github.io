package main

import (
	"fmt"
	"log"
	"packageTest/add" //导入 add 包 这个包必须放在$GOPATH下才可
	"packageTest/simpleinterest"
)

var p, r, t = 5000.0, 10.0, 1.0

/*
* init function to check if p, r and t are greater than zero
 */
func init() {
	println("Main package initialized")
	if p < 0 {
		log.Fatal("Principal is less than zero")
	}
	if r < 0 {
		log.Fatal("Rate of interest is less than zero")
	}
	if t < 0 {
		log.Fatal("Duration is less than zero")
	}
}

func main() {
	c := add.Add(1, 2)                      //调用 add 包中的 add 函数
	si := simpleinterest.Calculate(p, r, t) // 调用 simpleinterest 包的 Calculate 函数
	fmt.Println(c)
	fmt.Println("Simple interest is", si)
}
