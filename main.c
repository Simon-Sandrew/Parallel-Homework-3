#include <mpi.h>
#include <stdlib.h>
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
    int idx = 0;
    while(stride < size){
        ll curr = 0;

        if(rank % (2 * stride) > stride){
            int dest = rank - stride;
            MPI_Isend(recvbuf, 1, MPI_LONG_LONG, dest, 0, comm, &req);
            MPI_Wait(&req, MPI_STATUS_IGNORE);
        }

        if(rank % (2 * stride) == 0){
            MPI_Irecv(&curr, 1, MPI_LONG_LONG, rank + stride, 0, comm, &req);
            MPI_Wait(&req, MPI_STATUS_IGNORE);
            recvbuf[0] += curr;
        }
        
        stride *= 2;
    }
    MPI_Barrier(comm);

}

int main(int argc, char *argv[]) {
        int rank;
        int size;
        
        MPI_Init(&argc, &argv);
        
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
       

        const int BLOCK_SIZE = (1 << 30) / size;

        ll* in = calloc(BLOCK_SIZE, sizeof(ll));
        ll* rec = calloc(BLOCK_SIZE, sizeof(ll));
        ll val = BLOCK_SIZE * rank;
        
        for(int i = 0; i < BLOCK_SIZE; ++i){
            in[i] = val++;
        }

        MPI_P2P_REDUCE(in, rec, BLOCK_SIZE, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
       
        if(rank == 0){
            printf("%lld", out[0]);
        }
        MPI_Finalize();
        return 0;

}