#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "clockcycle.h"
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

    //until stride <  size 
    while(stride < size){
        ll curr = 0; 

        //we send if we are 2 * stride
        if(rank % (2 * stride) == stride){
            int dest = rank - stride;
            MPI_Isend(recvbuf, 1, MPI_LONG_LONG, dest, 0, comm, &req);
            MPI_Wait(&req, MPI_STATUS_IGNORE);
        }

        // rec on stride == 0
        if(rank % (2 * stride) == 0){
            MPI_Irecv(&curr, 1, MPI_LONG_LONG, rank + stride, 0, comm, &req);
            MPI_Wait(&req, MPI_STATUS_IGNORE);
            recvbuf[0] += curr;
        }
        
        stride *= 2;
    }
    //blocks until all processes have reached this routine
    MPI_Barrier(comm);

}

int main(int argc, char *argv[]) {
    int rank;
    int size;
    
    MPI_Init(&argc, &argv);
    
    //need rank and size for later
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    

    //2^30th constant
    const int blck = 1073741824/ size;

    ll* in = calloc(blck, sizeof(ll));
    ll* rec = calloc(blck, sizeof(ll));

    const double freq =  512000000;

    //pre-comp value
    ll val = blck * rank;

    ll sum = 0;
    ll solution = 0;
    for(int i = 0; i < blck; ++i){
        in[i] = val++;
        sum += in[i];
    }

    unsigned long long start_time1=clock_now(); // dummy clock reads to init
    MPI_P2P_REDUCE(in, rec, blck, MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    unsigned long long end_time1=clock_now();   // dummy clock reads to init start1 = clock_now();

    unsigned long long start_time2=clock_now(); // dummy clock reads to init
    MPI_Reduce(&sum, &solution, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    unsigned long long end_time2=clock_now();   // dummy clock reads to init start1 = clock_now();

    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0){
        printf("Value P2P: %lld\n", rec[0]);
        printf("Value Norm: %lld\n", solution);
        printf("Time P2P: %llu\n", (double)(end_time1 - start_time1));
        printf("Time NORM: %llu\n", (double)(end_time2 - start_time2));
        printf("Time calc: %llu\n", (double)(end_time2 - start_time2) / (double)freq);

    }

    MPI_Finalize();
    return 0;

}