#include <mpi.h>
#include <math.h>
#include <stdio.h>
#define ll long long

void MPI_P2P_REDUCE(ll *sendbuf, ll *recvbuf, int count, int root, int argc, char* argv[]){

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    printf("%d", world_size);
    MPI_Init(NULL, NULL);
    //stride is the size of the groups. We start with single groups of 1. Then we reduce by increasing the size of the stride.
    int stride  = 1;
    int mod = 2 * stride;
    while(stride != world_size){
        mod = 2 * stride;
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if(rank % stride == 0 && rank % mod == 0){
            //if rank is divisble by strid AND MOD then it is a reciever, meaning evaluation will be done. 
            //not sure what address we recieve from but it exists



            ll* buff;
            MPI_Irecv(buff, 1, MPI_LONG_LONG, sendbuf + stride, 0, MPI_COMM_WORLD, MPI_REQUEST_NULL);
            //do nothing
        }
        else if(rank % mod == 0){
            //needs to send to the nearest the address of current rank - stride, so for pair (0, 4), (8, 12)... 4 sends to 0, 12 sends to 8.  
        }
        stride *= 2; 
        //before continuing we need to wait for all ranks to finish this iteration of the loop
        MPI_Finalize();
    }
}

int main(int argc, char *argv[]) {
        MPI_P2P_REDUCE(NULL, NULL, 0, 0, argc, argv);
        return 0;

}