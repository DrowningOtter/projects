#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

long long compare_count;
long long change_count;

//сортировка по неубыванию
int compare_for_straight_sort(const void *a, const void *b) {
	int first = *((int*)a);
	int second = *((int*)b);
	if (abs(first) > abs(second)) return 1;
	if (abs(first) < abs(second)) return -1;

	return 0;
}

//сортировка по неубыванию
int compare_for_reverse_sort(const void *a, const void *b) {
	int first = abs(*((int*)a));
	int second = abs(*((int*)b));
	if (first < second) return 1;
	if (first > second) return -1;

	return 0;
}

void reverse_array_generate(int n, int* a) {
	for (int i = 0; i < n; ++i)
	{
		a[i] = rand() * rand() * rand();
	}

	qsort(a, n, sizeof(int), compare_for_reverse_sort);
}

void straight_array_generate(int n, int* a) {
	for (int i = 0; i < n; ++i)
	{
		a[i] = rand() * rand() * rand();
	}

	qsort(a, n, sizeof(int), compare_for_straight_sort);
}

void random_array_generate(int n, int *a) {
	for (int i = 0; i < n; ++i)
	{
		a[i] = rand() * rand() * rand();
	}
}

void shell_sort(int n, int *a) {
	change_count = 0;
	compare_count = 0;

	for (int i = n; i > 0; i /= 2) {
		for (int k = i; k < n; ++k) {
			for (int j = k - i; j >= 0/* && a[j] > a[j + i]*/; j -= i) {
				if (++compare_count && abs(a[j]) > abs(a[j + i])) {
					int tmp = a[j];
					a[j] = a[j + i];
					a[j + i] = tmp;
					change_count++;
				} else {
					break;
				}
			}
		}
	}
}

void simple_selection_sort(int n, int *a) {
	int min_ind = 0;

	change_count = 0;
	compare_count = 0;

	for (int i = 0; i < n; ++i) {
		min_ind = i;
		
		for (int j = i + 1; j < n; ++j) {
			if (++compare_count && abs(a[j]) < abs(a[min_ind])) {
				min_ind = j;
			}
			// compare_count++;
		}

		int tmp = a[i];
		a[i] = a[min_ind];
		a[min_ind] = tmp;
		change_count++;
	}
}

void print_array(int n, int *a) {
	for (int i = 0; i < n; ++i)
	{
		if (i == n - 1) {
			printf("%d", a[i]);
			break;
		}
		printf("%d, ", a[i]);
	}

	printf("\n");
}

int main(void) {
	srand(time(NULL));

	printf("Enter length of array:");
	int len;
	scanf("%d", &len);

	int *arr = malloc(len * sizeof(int));

	//analysis of selection sort
	random_array_generate(len, arr);
	simple_selection_sort(len, arr);
	printf("change_count, compare_count for simple selection sort on random dataset1 is %lld %lld\n", change_count, compare_count);

	random_array_generate(len, arr);
	simple_selection_sort(len, arr);
	printf("change_count, compare_count for simple selection sort on random dataset2 is %lld %lld\n", change_count, compare_count);

	straight_array_generate(len, arr);
	simple_selection_sort(len, arr);
	printf("change_count, compare_count for simple selection sort on straight sorted dataset is %lld %lld\n", change_count, compare_count);

	reverse_array_generate(len, arr);
	simple_selection_sort(len, arr);
	printf("change_count, compare_count for simple selection sort on reverse sorted dataset is %lld %lld\n", change_count, compare_count);

	//analysis of shell sort
	random_array_generate(len, arr);
	shell_sort(len, arr);
	printf("change_count, compare_count for shell sort on random dataset1 is %lld %lld\n", change_count, compare_count);

	random_array_generate(len, arr);
	shell_sort(len, arr);
	printf("change_count, compare_count for shell sort on random dataset2 is %lld %lld\n", change_count, compare_count);

	straight_array_generate(len, arr);
	shell_sort(len, arr);
	printf("change_count, compare_count for shell sort on straight sorted dataset is %lld %lld\n", change_count, compare_count);

	reverse_array_generate(len, arr);
	shell_sort(len, arr);
	printf("change_count, compare_count for shell sort on reverse sorted dataset2 is %lld %lld\n", change_count, compare_count);

	free(arr);
}