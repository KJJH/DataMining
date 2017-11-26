#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX 10

// Set out-links for all pages -- random
void set_page(int page[][MAX])
{
	int i, j;

	srand(time(NULL));
	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			page[i][j] = (i == j) ? 0 : (rand() % 4) == 1;    // 랜덤하게 세팅.
		}
	}
}

void put_page(int page[][MAX])
{
	int i, j;

	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			printf("  %d", page[i][j]);
		}
		putchar('\n');
	}
}

void pagerank(int page[][MAX], double rank[MAX], double dfactor)   // 페이지 랭크 함수에서 랭킹 계산.
{
	double avg = 0.;
	double preAvg = 0.;
	double sub = 0.;
	int i, j, k;
	int nout[MAX] = { 0 };	// number of out-links for page i
	double pr = 0.;  // pagerank

	for (i = 0; i < MAX; i++) rank[i] = 1.0;	// initalize

	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			if (page[i][j]) nout[i]++;
		}
	}

	for (k = 0; k < 990; k++) {	// max. iteration is 99
		for (i = 0; i < MAX; i++) {
			// pr (PageRank) 값 계산
			if (nout[i] == 0)  // dangling link 계산에서 제외
				continue;

			for (j = 0; j < MAX; j++) {
				if (page[j][i] != 0) {
					pr += rank[j] / nout[j];
				}
			}
			rank[i] = (1.0 - dfactor) + dfactor * pr;
			pr = 0.0;

		}
		
		// rank[i] 값에 변동이 없거나 미미할 때 break
		printf("[ %d ]\n", k);
		for (j = 0; j < MAX; j++)
			avg += rank[j];
		avg /= MAX;
		sub = avg - preAvg;
		printf("Page Rank Average : %f\n", avg);
		printf("Avg( %f ) - PreAvg( %f ) = %f\n", avg, preAvg, sub);
		preAvg = avg;
		avg = 0.;

		// rank값을 평균을 내서 이전 평균과의 차이를 비교
		if (round(preAvg * 1000000) / 1000000 == 1.0) {  // 평균이 1이 되는 경우 break 
			printf("One\n");
			break; 
		}
		if (round(sub * 10000000000) / 10000000000 == 0.0) {  // 소수점 10자리까지 봐서 변화가 없으면 break
			printf("Zero\n");
			break;
		}
	}

	for (i = 0; i < MAX; i++) {
		avg += rank[i];
		printf("%d : %5.2f\n", nout[i], rank[i]);
	}
	printf("Page Rank Average : %f\n", avg/MAX);
}

int main() {
	int page[MAX][MAX];
	double rank[MAX] = { 1.0 };

	set_page(page);
	put_page(page);
	pagerank(page, rank, 0.85);
}
