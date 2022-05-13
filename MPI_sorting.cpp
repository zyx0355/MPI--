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
	// ÿ�ν�һ��Ԫ���͵�ĩβ��n��Ԫ�أ�ִ��n��
	for (i = 0; i < n; ++i) {
		// ֮ǰ��ѭ���Ѿ���i��Ԫ���͵�ĩβ������Ҫ�ٴαȽϣ��ʼ�ȥ����Ϊ����һ��Ԫ�رȽϣ�Ϊ�˱���������ʼ�һ
		for (j = 0; j < n - i - 1; ++j) {
			// �����ǰ��Ԫ�رȺ�һ��Ԫ��С���ͽ���
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
