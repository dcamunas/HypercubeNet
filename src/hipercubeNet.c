#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <definitions.h>
#include <math.h>
#include <mpi.h>


FILE *open_file(const char *path, const char *mode);
int load_data(long double *data);
int check_size(const int size, const int D, const int numbers_n);
void add_numbers(long double *data, const int size);
void get_neighbors(const int rank, int *neighbors, const int D);
long double calculate_max(const int rank, const int D, long double my_number, int *neighbors);
void print_max_number(const int rank, long double max_number);


int main(int argc, char *argv[])
{
    /*Variables*/
    int rank, size, D, numbers_n, finish;
    long double number, max_number;
    long double *data;
    int *neighbors;

    /* Initialize MPI program */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Initialize value of variable */
    D = (int)log2(size);
    data = malloc(size * DATA_SIZE);
    neighbors = malloc(D * NEIGHBOR_SIZE);

    if (rank == FIRST_RANK)
    {
        /*Get quantity of numbers*/
        numbers_n = load_data(data);

        finish = check_size(size, D, numbers_n);

        if (finish != TRUE)
        {
            add_numbers(data, size);
        }
    }

    /* Indicate to all nodes, if program finish or no */
    MPI_Bcast(&finish, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (finish != TRUE)
    {
        /*Receive number by the rank*/
        MPI_Recv(&number, 1, MPI_LONG_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        
        get_neighbors(rank, neighbors, D);

        max_number = calculate_max(rank, D, number, neighbors);

        print_max_number(rank,max_number);


    }

    /* Finalize MPI program */
    MPI_Finalize();

    return EXIT_SUCCESS;
}

/* Load data (numbers) from datos.dat */
int load_data(long double *data)
{
    FILE *file = open_file(DATA_PATH, READ_MOD);
    char line[MAX_SIZE];
    char *token;
    int i;

    fgets(line, sizeof(line), file);
    fclose(file);

    data[0] = atof(strtok(line, SEPARATOR));

    for (i = 1; (token = strtok(NULL, SEPARATOR)) != NULL; i++)
    {
        data[i] = atof(token);
    }

    return i;
}

/* Open file */
FILE *open_file(const char *path, const char *mode)
{
    FILE *file;
    if ((file = fopen(path, mode)) == NULL)
    {
        fprintf(stderr, "[X] RANK[0]: Error opening file.\n");
        exit(EXIT_FAILURE);
    }
    return file;
}

/* Check hipercube's size (nº nodes) */
int check_size(const int size, const int D, const int numbers_n)
{
    int finish = FALSE;
    if (size != numbers_n)
    {
        fprintf(stderr, "[X] RANK[0]: Error, quantity of numbers (%d) is diferent at hipercube size (%d | D = %d)\n",
                numbers_n, size, D);
        finish = TRUE;
    }
}

/* Add number to nodes (ranks) */
void add_numbers(long double *data, const int size)
{
    int i;
    long double number;

    for(i = 0; i < size; i++)    
    {
        number = data[i];
        MPI_Send(&number, 1, MPI_LONG_DOUBLE, i, SEND_TAG, MPI_COMM_WORLD);
        
        /*Print number send to rank i*/
        printf("[X] RANK[%d] --> %.2Lf\n", i, number);

        
    }
    
    free(data);
}

/* Get rank's neighbours */
void get_neighbors(const int my_rank, int *neighbors, const int D)
{
    int i, his_rank;

    for(i = 0; i < D; i++)
    {
        /* Depending on the dimension, the position of the bit is modified with one */
        his_rank = 1 << i;
        
        /*my_rank XOR his_rank*/
        neighbors[i] = my_rank ^ his_rank;
    }
}

/*Calculate the maxium value*/
long double calculate_max(const int rank, const int D, long double my_number, int *neighbors)
{
    int i;
    long double his_number;

    for(i = 0; i < D; i++)
    {
        MPI_Send(&my_number, 1, MPI_LONG_DOUBLE, neighbors[i], SEND_TAG, MPI_COMM_WORLD);
        MPI_Recv(&his_number, 1, MPI_LONG_DOUBLE, neighbors[i], MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        
        /* Get the max number */
        my_number = (his_number < my_number ? my_number : his_number);    
    }

    return my_number;
}

/*Print the maximum value by First Rank (Rank == 0)*/
void print_max_number(const int rank, long double max_number)
{
    if (rank == FIRST_RANK)
    {
        printf("\n[X] RANK[%d]: The maximum value is: %.2Lf\n", rank, max_number);

        /*End message*/
        printf("----------------------------------------------------------------------------------------\n");
        printf("\t\t\t***** PROGRAM FINALIZED *****\n\n\n");
    }
}