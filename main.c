#include <iostream>
#include <mpi.h>
#include <math.h>
#define ll long long

void MPI_P2P_REDUCE(ll *sendbuf, ll *recvbuf, int count, int root, int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int height = log2(argc);

    //number of times it will loop
    int stride  = 1;
    while(height >= 0){

        int mod = 2 * stride;
        for(int i = 0; i < argc; i+= stride){
            if(i % mod == 0){
                //this is a reciever, recieve from i + stride
                MPI_Irecv(recvbuf, count, MPI_LONG_LONG, i + stride, 0, MPI_COMM_WORLD, MPI_REQUEST_NULL);

            }else{
                //send to i - stride,
            }
        }
    }





}

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;
    return 0;

}
