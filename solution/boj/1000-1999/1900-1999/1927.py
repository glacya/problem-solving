# BOJ 1927 - 최소 힙
# 난이도: Silver II
# 푼 날짜: 2020-08-18
# https://www.acmicpc.net/problem/1927

import sys
input = sys.stdin.readline

class Minheap():
    def __init__(self,args=None):
        self.heap = [None]
        if args is not None:
            self.heap.extend(args)
        self.size = len(self.heap) - 1

    def isempty(self):
        return self.size == 0

    def compare(self, a, b):
        if self.heap[a] > self.heap[b]:
            return True
        else:
            return False
        
    def swap(self, i, j):
        mid = self.heap[i]
        self.heap[i] = self.heap[j]
        self.heap[j] = mid

    def sink(self, j):
        if 2*j > self.size:
            return
        elif 2*j == self.size:
            if self.compare(j, 2*j):
                self.swap(j, 2*j)
            return
        else:
            if self.compare(j, 2*j):
                if self.compare(2*j, 2*j+1):
                    self.swap(j,2*j+1)
                    self.sink(2*j+1)
                    return
                else:
                    self.swap(j,2*j)
                    self.sink(2*j)
                    return
            elif self.compare(j, 2*j+1):
                self.swap(j, 2*j+1)
                self.sink(2*j+1)
                return
            else:
                return
                

    def rise(self, j):
        i = j
        while i//2 != 0 and self.compare(i//2, i):
            self.swap(i//2, i)
            i = i//2

    def add(self, element):
        self.size += 1
        self.heap.append(element)
        self.rise(self.size)

    def removeMin(self):
        if self.isempty():
            print(0) 
            return
        print(self.heap[1])
        self.heap[1] = None
        self.swap(1, self.size)
        self.heap.pop()
        self.size -= 1
        self.sink(1)

n = int(input())
minhp = Minheap()
for _ in range(n):
    k = int(input())
    if k == 0:
        minhp.removeMin()
    else:
        minhp.add(k)
