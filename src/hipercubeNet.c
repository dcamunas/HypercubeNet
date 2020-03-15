#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <definitions.h>
#include <math.h>
#include <mpi.h>

FILE *open_file(const char *path, const char *mode);
int load_data(long double *data);
int check_size(int size, int D, int numbers_n);
void add_numbers(long double *data, int size, int rank);


int main(int argc, char *argv[])
{
    int rank, size, D, numbers_n, finish;
    long double number, max_number;
    long double *data;
    int *neighbors = malloc(NEIGHBORS_SIZE);

    /* Initialize MPI program */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Get size value */
    D = (int)log2(size);
    data = malloc(size*sizeof(long double));

    if (rank == FIRST_RANK)
    {
        /*Get quantity of numbers*/
        numbers_n = load_data(data);

        finish = check_size(size, D, numbers_n);

        if (finish != TRUE)
        {
           add_numbers(data, size, rank);
        }
    }

    MPI_Bcast(&finish, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (finish != TRUE)
    {
        MPI_Recv(&number, 1, MPI_LONG_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        printf("[X] RANK[%d] --> %.2Lf\n", rank, number);
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

int check_size(int size,int D, int numbers_n)
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
void add_numbers(long double *data, int size, int rank)
{
    int i;
    long double number;

    for (i = 0; i < size; i++)
    {
        number = data[i];
        MPI_Send(&number, 1, MPI_LONG_DOUBLE, i, SEND_TAG, MPI_COMM_WORLD);
    }

    free(data);
}
