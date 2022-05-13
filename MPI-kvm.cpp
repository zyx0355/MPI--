#include <iostream> 
#include <mpi.h> 
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#pragma warning(disable:4996)
#define TOTAL_SIZE 100000
#define size TOTAL_SIZE/(process_num-2)
int max1=0, max2=0, max3=0;
int min1 = 99999, min2 = 99999, min3 = 99999;
int original_array[TOTAL_SIZE],sorted_array[TOTAL_SIZE];
int cmpfunc(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}
int array_test[TOTAL_SIZE];
int array_size_test;
int main(int argc, char* argv[])
{
	int world_rank;
	int process_num;
	MPI_Status status;
	MPI_Request request;
	MPI_Init(&argc, &argv);
	//int namelen;
	//char processor_name[MPI_MAX_PROCESSOR_NAME];//设备名
	//MPI_Get_processor_name(processor_name, &namelen);
	//int color;
	//if (processor_name == "node2") {
	//	color = 0;
	//}
	//if (processor_name == "slave0")
	//{
	//	color = 0;
	//}
	//if (processor_name == "slave1")
	//{
	//	color = 0;
	//}
	//if (processor_name == "node1") {
	//	color = 1;
	//}
	//if (processor_name == "slave2")
	//{
	//	color = 1;
	//}
	//if (processor_name == "slave3")
	//{
	//	color = 1;
	//}
	int c;
	srand(time(NULL));
	
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);
	MPI_Comm SplitWorld;
	MPI_Comm_split(MPI_COMM_WORLD, world_rank%2, world_rank, &SplitWorld);
	int row_size;
	int row_rank;
	MPI_Comm_rank(SplitWorld, &row_rank);
	MPI_Comm_size(SplitWorld, &row_size);
	if (world_rank == 0)
	{
		printf("This is the unsorted array: ");
		for (c = 0; c < TOTAL_SIZE; c++)
		{
			original_array[c] = rand() % TOTAL_SIZE;
			if (original_array[c] > max1)
				max1 = original_array[c];
			if (original_array[c] < min1)
				min1 = original_array[c];
			//printf("%d ", original_array[c]);
		}
		printf("%d ", max1);
		printf("%d ", min1);
		printf("\n");
		printf("\n");
		//initm();
		int  k1 = 0, k2 = 0;//初始时小于和大于中位数元素数目

		int mid1 = (max1+min1) / 2;
			//original_array[0]+ original_array[TOTAL_SIZE-1];
		printf("%d第一次分界点", mid1);
		printf("\n"); printf("\n");
		for (int i = 0; i < TOTAL_SIZE; i++) {
			if (original_array[i] < mid1)
				++k1;
			else
				++k2;
		}
	/*	array_aize[0] = k1;
		array_aize[1] = k2;*/

		int* mid_result1 = new int[k1];
		int* mid_result2 = new int[k2];
		int s1 = 0, s2 = 0;
		printf("%d划分后数据1大小", k1);
		printf("\n");
		printf("\n");
		printf("%d划分后数据2大小", k2);
		printf("\n");
		printf("\n");
		for (int i = 0; i < TOTAL_SIZE; i++) {
			if (original_array[i] < mid1) {
				mid_result1[s1] = original_array[i];
				s1++;
			}
			else {
				mid_result2[s2] = original_array[i];
				s2++;
			}
		}
		printf("开始排序");
		
	    MPI_Send(mid_result2, k2, MPI_INT, 1, 0, MPI_COMM_WORLD);
		array_size_test = k1;
		//printf("%d", array_size_test);
		for (int j = 0; j < k1; j++) {
			array_test[j] = mid_result1[j];
		}
		
	}
	if (row_rank == 0 && world_rank == 1)
	{
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		int number_amount;
		MPI_Get_count(&status, MPI_INT, &number_amount);
		int* mid_result = new int[number_amount];
		MPI_Recv(mid_result, number_amount, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		printf("%d", number_amount);
		int s1 = 0, s2 = 0;
		int k3 = 0, k4 = 0;
		printf("\n");
		printf("\n");
		for (int k = 0; k < number_amount; k++) {
			if (mid_result[k] > max2)
				max2 = mid_result[k];
			if(mid_result[k] < min2)
				min2 = mid_result[k];
		}
		int mid = (min2 + max2) / 2;
		printf("%d第二个分界点", mid);
		printf("\n");
		printf("\n");
			//mid_result[0] + mid_result[number_amount - 1];
		for (int i = 0; i < number_amount; i++) {
			if (mid_result[i] < mid)
				++k3;
			else
				++k4;
		}
		printf("%d划分后数据1大小", k3);
		printf("\n");
		printf("\n");
		printf("%d划分后数据2大小", k4);
		printf("\n");
		printf("\n");
		int* mid_result1 = new int[k3];
		int* mid_result2 = new int[k4];
		for (int i = 0; i < number_amount; i++) {
			if (mid_result[i] < mid) {
				mid_result1[s1] = mid_result[i];
				s1++;
				//printf("%d ", mid_result[i]);
			}
			else {
				mid_result2[s2] = mid_result[i];
				s2++;
				//printf("%d ", mid_result[i]);
			}
		}
		/*for (int i = 0; i < s1; i++) {
			printf("%d ", mid_result1[i]);
		}*/
		/*for (int i = 0; i < s2; i++) {
			printf("%d ", mid_result2[i]);
		}*/

		MPI_Send(mid_result1, k3, MPI_INT, 1, 13, SplitWorld);
		MPI_Recv(mid_result1, k3, MPI_INT, 1, 14, SplitWorld, &status);
		MPI_Send(mid_result2, k4, MPI_INT, 2, 15, SplitWorld);
		MPI_Recv(mid_result2, k4, MPI_INT, 2, 16, SplitWorld, &status);
		int amount = k3 + k4;
		int* result = new int[amount];
		for (int i = 0; i < amount; i++) {
			if (i < k3) {
				result[i] = mid_result1[i];
			//	printf("%d ", result[i]);
			}
			else {
				result[i] = mid_result2[i - k3];
			//	printf("%d ", result[i]);
			}
				}
		/*for(int k=0;k< amount;k++)
			printf("%d ", result[k]);*/
		MPI_Send(result, amount, MPI_INT, 0, 4, MPI_COMM_WORLD);
	}
if (row_rank == 0 && world_rank == 0)
	{
		int* mid_result = new int[array_size_test];
		for (int j = 0; j < array_size_test; j++) {
			mid_result[j] = array_test[j];
			if (array_test[j] > max3)
				max3 = array_test[j];
			if (array_test[j] < min3)
				min3 = array_test[j];
		}
		printf("%d", array_size_test);
		printf("\n");
		printf("\n");
		int s1 = 0, s2 = 0;
		int k3 = 0, k4 = 0;
		int mid = (min3 + max3) / 2;
		printf("%d ", mid);
		for (int i = 0; i < array_size_test; i++) {
			if (mid_result[i] < mid)
				++k3;
			else
				++k4;
		}
		int* mid_result1 = new int[k3];
		printf("%d ", k3);
		int* mid_result2 = new int[k4];
		for (int i = 0; i < array_size_test; i++) {
			if (mid_result[i] < mid) {
				mid_result1[s1] = mid_result[i];
				s1++;
			}
			else {
				mid_result2[s2] = mid_result[i];
				s2++;
			}
		}
			/*for (int i = 0; i < s1; i++) {
			printf("%d ", mid_result1[i]);
		}
			printf("\n");
			printf("\n");*/
		/*for (int i = 0; i < s2; i++) {
			printf("%d ", mid_result2[i]);
		}
		printf("\n");
		printf("\n");*/
		MPI_Send(mid_result1, k3, MPI_INT, 1, 13, SplitWorld);
		MPI_Recv(mid_result1, k3, MPI_INT, 1, 14, SplitWorld, &status);
		MPI_Send(mid_result2, k4, MPI_INT, 2, 15, SplitWorld);
		MPI_Recv(mid_result2, k4, MPI_INT, 2, 16, SplitWorld, &status);
		int amount = k3 + k4;
		array_size_test = amount;
		int* result = new int[amount];
		for (int i = 0; i < amount; i++) {
			if (i < k3) 
				result[i] = mid_result1[i];
			else 
				result[i] = mid_result2[i - k3];
		}
		for (int j = 0; j < array_size_test; j++) {
			array_test[j] = result[j];
			//printf("%d ", result[j]);
		}
	}
	if(row_rank==1) {
		MPI_Probe(0, 13, SplitWorld, &status);
		int number_amount;
		MPI_Get_count(&status, MPI_INT, &number_amount);
		printf("%d result", number_amount);
		int* mid_result = new int[number_amount];
		MPI_Recv(mid_result, number_amount, MPI_INT, 0, 13, SplitWorld, &status);
		qsort(mid_result, number_amount, sizeof(int), cmpfunc);
		MPI_Send(mid_result, number_amount, MPI_INT, 0, 14, SplitWorld);
	}
	if (row_rank == 2) {
		MPI_Probe(0, 15, SplitWorld, &status);
		int number_amount;
		MPI_Get_count(&status, MPI_INT, &number_amount);
		printf("%d result", number_amount);
		int* mid_result = new int[number_amount];
		MPI_Recv(mid_result, number_amount, MPI_INT, 0, 15, SplitWorld, &status);
		qsort(mid_result, number_amount, sizeof(int), cmpfunc);
		MPI_Send(mid_result, number_amount, MPI_INT, 0, 16, SplitWorld);
	}
	if (world_rank == 0) {
		int number_amount1 = array_size_test;
		int* fin_result1 = new int[number_amount1];
		for (int j = 0; j < array_size_test; j++) {
			fin_result1[j] = array_test[j];
		}
		printf("%d数据1大小 ", number_amount1);

		MPI_Probe(1, 4, MPI_COMM_WORLD, &status);
		int number_amount2;
		MPI_Get_count(&status, MPI_INT, &number_amount2);
		int* fin_result2 = new int[number_amount2];
		printf("%d数据2大小 ", number_amount2);
		MPI_Recv(fin_result2, number_amount2, MPI_INT, 1, 4, MPI_COMM_WORLD, &status);
		if (fin_result1[0] > fin_result2[0]) {
			for (int i = 0; i < TOTAL_SIZE; i++) {
				if (i < number_amount2)
					sorted_array[i] = fin_result2[i];
				else
					sorted_array[i] = fin_result1[i - number_amount2];
			}
		}
		else {
			for (int i = 0; i < TOTAL_SIZE; i++) {
				if (i < number_amount1)
					sorted_array[i] = fin_result1[i];
				else
					sorted_array[i] = fin_result2[i - number_amount1];
			}
		}
		/*for (c = 0; c < TOTAL_SIZE; c++)
		{
			printf("%d ", sorted_array[c]);
		}*/
	}

	if (world_rank == 0)
	{
		printf("This is the sorted array: ");
		for (c = 0; c < TOTAL_SIZE; c++)
		{
			printf("%d ", sorted_array[c]);
		}
		printf("\n");
		printf("\n");
		FILE* fp = fopen("result.txt", "w");  //以文本模式按只写方式打开文件
		if (fp == NULL)                    //判断文件是否成功打开
		{
			printf("File open failed!\n");
			exit(0);
		}
		for (int i = 0; i < TOTAL_SIZE; i++)    
		{
			fprintf(fp, "%d  ", sorted_array[i]);
			fputc('\n', fp);
		}
		fclose(fp);                //关闭文件
	}
	MPI_Finalize();
	return 0;
}
