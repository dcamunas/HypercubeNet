void get_neighbors(const int my_rank, int *neighbors, const int D)
{
    int i, his_rank;

    for(i = 0; i < D; i++)
    {
        /* Modificate the position by dimension*/
        his_rank = 1 << i;
        neighbors[i] = my_rank ^ his_rank;
    }
}
