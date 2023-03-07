#include <mpi.h>
#include <math.h>
#include <stdio.h>
#define ll long long

void MPI_P2P_REDUCE(ll *sendbuf, ll *recvbuf, int count, MPI_Datatype datatype, int root, MPI_Comm comm){
    int rank;
    int size;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    MPI_Request req;
    recvbuf[0] = 0;
    for(int i = 0; i < count; ++i){
        recvbuf[0] += sendbuf[i];
    }

    int stride = 1;
    while(stride < size){
        ll curr = 0;
        if(rank % (2 * stride) == 0){
            MPI_Irecv(&curr, 1, MPI_LONG_LONG, rank + stride, 0, comm, &req);
            MPI_Wait(&req, MPI_STATUS_IGNORE);
            recvbuf[0] += curr;
        }
        
        if(rank % (2 * stride) == stride){
            int dest = rank - stride;
            MPI_Isend(recvbuf, 1, MPI_LONG_LONG, dest, 0, comm, &req);
            MPI_Wait(&req, MPI_STATUS_IGNORE);
        }


        stride *= 2;
    }

}

int main(int argc, char *argv[]) {
        ll* curr = calloc(sizeof(ll), 16);
        for(int i = 0; i < 16; ++i){
            curr[i] = i;
        }
        MPI_Init(&argc, &argv);
        MPI_P2P_REDUCE(curr, curr, 16, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        MPI_Wait(NULL, NULL);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if(rank == 0){
            printf("%lld", curr[0]);
        }
        return 0;

}