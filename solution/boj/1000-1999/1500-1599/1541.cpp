// BOJ 1541 - 잃어버린 괄호
// 난이도: Silver II
// 푼 날짜: 2021-02-23
// https://www.acmicpc.net/problem/1541

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char expr[51];
	scanf("%s", expr, 51);
	int sum = 0;
	int op = 1;
	int numlen = 0;

	for (int i = 0; i < 51; i++) {
		if (expr[i] == '-') {
			expr[i] = 0;
			sum += op * atoi(expr + i - numlen);
			numlen = 0;
			op = -1;
		}
		else if (expr[i] == '+') {
			expr[i] = 0;
			sum += op * atoi(expr + i - numlen);
			numlen = 0;
		}
		else {
			if (!expr[i]) {
				sum += op * atoi(expr + i - numlen);
				break;
			}
			numlen++;
		}
	}

	printf("%d", sum);
	return 0;
}
