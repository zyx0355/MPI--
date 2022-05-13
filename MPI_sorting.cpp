#include <iostream> 
#include <mpi.h> 
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#define TOTAL_SIZE 1000
#define size TOTAL_SIZE/process_num

void bubblingSort(int arr[], int n) {
	int i, j, temp;
	// 每次将一个元素送到末尾，n个元素，执行n次
	for (i = 0; i < n; ++i) {
		// 之前的循环已经将i个元素送到末尾，不需要再次比较，故减去，因为跟后一个元素比较，为了避免溢出，故减一
		for (j = 0; j < n - i - 1; ++j) {
			// 如果当前的元素比后一个元素小，就交换
			if (arr[j] > arr[j + 1]) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
	//printf("no use?");
}

int main(int argc, char* argv[])
{
	int RankID;
	int original_array[TOTAL_SIZE], sorted_array[TOTAL_SIZE];
	int c;
	srand(time(NULL));
	int world_rank;
	int process_num;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	if (world_rank == 0)
	{
		printf("This is the unsorted array: ");
		for (c = 0; c < TOTAL_SIZE; c++)
		{
			original_array[c] = rand() % TOTAL_SIZE;
			printf("%d ", original_array[c]);
		}
		printf("\n");
		printf("\n");
	}

	int *sub_array = (int *)malloc(size * sizeof(int));
	MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

	bubblingSort(sub_array, size);

	MPI_Gather(sub_array, size, MPI_INT, sorted_array, size, MPI_INT, 0, MPI_COMM_WORLD);
	if (world_rank == 0)
	{
		printf("This is the sorted array: ");
		for (c = 0; c < TOTAL_SIZE; c++)
		{
			printf("%d ", sorted_array[c]);
		}
		printf("\n");
		printf("\n");
	}

	MPI_Finalize();
	return 0;
}
