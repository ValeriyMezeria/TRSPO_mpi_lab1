#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0
#define ARR_SIZE 10000000

long * array = NULL;

long * getArr()
{
    if (!array)
    {
        array = malloc(sizeof(long) * ARR_SIZE);
        for (int i = 0; i < ARR_SIZE; i++)
            array[i] = i;

        return array;
    }
    else
    {
        return array;
    }
}

int main (int argc, char *argv[])
{
    int numtasks, taskid;
    long sum = 0;
    long * arr = getArr();
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    
    int block_size = ARR_SIZE / numtasks;

    if (taskid == MASTER)
    {
        printf("Num tasks: %d\n", numtasks);
        sum = 0;
        for (int i = 0; i < ARR_SIZE; i++)
            sum += i;

        printf("Correct sum: %li\n", sum);

        long tmp = 0;
        long total_sum = 0;
        double start_time = MPI_Wtime();
        for (int i = 1; i < numtasks; i++)
        {
            MPI_Recv(&tmp, 1, MPI_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("Tmp: %li\n", tmp);
            total_sum += tmp;
        }

        for (int i = ARR_SIZE - 1; i > ARR_SIZE - ARR_SIZE % numtasks - block_size - 1; i--)
            total_sum += arr[i];
        double stop_time = MPI_Wtime();
        printf("Total sum: %li\nTotal time:%f\n", total_sum, stop_time - start_time);
    }
    else
    {
        //printf("Task: %d\n", taskid);
        taskid--;
        long sum = 0;
        if (arr)
        {  
            for (int i = taskid * block_size; i < taskid * block_size + block_size; i++)
                sum += arr[i];
            MPI_Send(&sum, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    
    if (arr)
        free(arr);
}
