package main

import (
	"fmt"
	"math/rand"
	"time"
	"flag"
)

func main(){
	// Usage: go run homework8_1.go -int <array num>
	var argint = flag.Int("int",0,"int flag")
	flag.Parse()
	n := *argint

	a := make([][]int,n)
	b := make([][]int,n)
	c := make([][]int,n)
	for i:=0;i<n;i++ {
		a[i] = make([]int,n)
		b[i] = make([]int,n)
		c[i] = make([]int,n)
	}
	for i:=0;i<n;i++ {
		for j:=0;j<n;j++{
			a[i][j] = rand.Intn(100) //ランダム
			b[i][j] = rand.Intn(100)
			c[i][j] = 0
//			fmt.Println(a[i][j],b[i][j],c[i][j])
		}
	}
	for kaisuu := 0;kaisuu<4;kaisuu++ {
		/* 計測開始 */
		start := time.Now()
	
		/* ここの順番を入れ替える */
		for i:=0;i<n;i++ {
			for j:=0;j<n;j++ {
				for k:=0;k<n;k++ {
					c[i][j] += a[i][k] * b[k][j]
				}
			}
		}

		/* 計測終了 */
		end := time.Now()
		fmt.Printf("%v*%[1]v行列\n",n)
		fmt.Printf("実行時間: %vms\n",(end.Sub(start).Milliseconds()))
	}	
}
