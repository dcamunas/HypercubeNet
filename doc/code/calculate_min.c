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
