//Zhou trying to use MPICH2
#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define DIM 16 //16x16 grid
#define GENERATIONS 64 

//global constat initial glider, copied from Randal's data
int globalgrid[ 256 ] = { 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


void printGrid(); //print out the grid
void printAux (int *grid, int sz);//print out aux local Grid for debug
void nextGrid(int *grid, int *hi, int *lo, int rows); //Determine whether one cell can survive
int prevProc(int a, int sz);
int nextProc(int a, int sz);
//int **alloc_2d_int(int rows,int cols); //malloc the 2D array, decided to use one dimentional array to save time
///////////////////
//                //
//main part starts//
//////////////////

int main (int argc, char **argv) {
 //initialize MPI
 MPI_Status stat;
 int size, rank;
 int rc;
 rc = MPI_Init(&argc, &argv);
 if (rc!=MPI_SUCCESS){
   printf("Error startging MPI. Tasks failed");
   MPI_Abort(MPI_COMM_WORLD,rc);
  }
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);

// each process is managing subsize of rows
int subsize = DIM/size; 

//Enter 64 cycles
int cycle;
//each process has a local copy of grid
int *grid = (int*) malloc(DIM*subsize*sizeof(int));
//each process has a local copy of edge rows
int *bottom = (int*) malloc(DIM*sizeof(int));
int *top = (int*)malloc(DIM*sizeof(int));
// copy the global data to local grids
int c1,c2;
for (c1=0;c1<subsize;c1++){
   for(c2=0;c2<DIM;c2++){
      grid[c1*DIM+c2] = globalgrid[(rank*subsize+c1)*DIM+c2];
   }
 }
// end of MPI intialization
if (rank ==0){
  printf("Initial grid:\n----------------\n");
  printGrid();
}

for (cycle = 0;cycle < GENERATIONS; cycle++){
 // to handle the special situation that only one processor
 if (size ==1){
   int proc1;
   for (proc1 = 0;proc1<DIM;proc1++){
   // copy the bottom and top row to process 1
    top[proc1]= grid[(DIM-1)*DIM+proc1];
    bottom[proc1]=grid[proc1];
    }
   nextGrid(grid, top, bottom, subsize);
   int proci;
   for (proci=0;proci<DIM*DIM;proci++){
     globalgrid[proci]=grid[proci];
   }
   printf("Iteration %d\n---------------\n", cycle);
   printGrid();   
 } else {
 //passing dependent rows to other MPI processes;
 //First if-else pair, each process passes its toprow to aux-bottom of the previous process
 
 if(rank!=0){
   MPI_Recv(bottom,DIM,MPI_INT,nextProc(rank,size),1,MPI_COMM_WORLD, &stat);
 } 
   MPI_Send(&grid[0],DIM,MPI_INT,prevProc(rank,size),1,MPI_COMM_WORLD);
 if (rank ==0){
   MPI_Recv(bottom,DIM,MPI_INT,nextProc(rank,size),1,MPI_COMM_WORLD, &stat);
 }

 //Second if-else pair, each process passes its bottom row to aux-top of the next process
 
 if(rank!=0){
   MPI_Recv(top,DIM,MPI_INT,prevProc(rank,size),2,MPI_COMM_WORLD, &stat);
 }  
  MPI_Send(&grid[DIM*(subsize-1)],DIM,MPI_INT,nextProc(rank,size),2,MPI_COMM_WORLD);
 if (rank ==0){
   MPI_Recv(top,DIM,MPI_INT,prevProc(rank,size),2,MPI_COMM_WORLD, &stat);
 }
 
//the process of exchanging the dependencies completed
// it started with process 0, and end with process 0, so is synchronized
//update the local grid;
 nextGrid(grid, top, bottom, subsize);
//communicate the grid updates from processes 1..n-1 to process 0 then print out
//also synchronize all the processes
int i, j,k;
 if (rank ==0){
   for (i =0;i<subsize;i++){
     for(j = 0;j<DIM; j++){
       globalgrid[i*DIM+j] = grid[i*DIM+j];
    }
   } 
   for (k = 1; k<size;k++){
       MPI_Recv(&globalgrid[k*subsize*DIM],DIM*subsize,MPI_INT,k,0,MPI_COMM_WORLD,&stat);
   }
   printf("Iteration %d\n---------------\n", cycle);
   printGrid();
 } else {
       MPI_Ssend(&grid[0],DIM*subsize,MPI_INT,0,0,MPI_COMM_WORLD);
     }
  }
 }
 //Terminate MPI, release space
 MPI_Finalize();
 free(top);
 free(bottom);
 free(grid);
}

///////////////////
//                //
//main part ends//
//////////////////

void printGrid(){ 
 int i ,j;
 for(i = 0;i<DIM;i++){
   for(j = 0;j<DIM;j++){
     printf("%d ",globalgrid[i*DIM+j]);
    }
    printf("\n");
   }
  printf("\n");
}
void printAux(int *agrid,int sz){ 
 int i ,j;
 for(i = 0;i<sz;i++){
   for(j = 0;j<DIM;j++){
     printf("%d ",agrid[i*DIM+j]);
    }
    printf("\n");
   }
  printf("\n");
}

int prevProc(int a, int sz){
 if(a ==0) return (sz-1);
 else return a-1;
}
int nextProc(int a, int sz){
 if (a == (sz-1)) return 0;
 else return a+1;
}

void nextGrid(int *tmpgrid, int *hi, int *lo, int rows){
// each process updates curret Grid based on the neighboring cells
 int i, j, score; //score: the number of surviving neighboring cells
 int (*temp)[DIM] = new int [rows+2][DIM];// combine the required data from top and bottom and currGrid into a local matrix
 int k,l; 
 for (k = 0;k<rows+2;k++){
   for(l = 0;l<DIM;l++){
     if (k ==0){
       temp[k][l]= hi[l];
     } else if (k == rows+1){
         temp[k][l] = lo[l];
     } else {temp[k][l] = tmpgrid[(k-1)*DIM+l];}
    }
  }
 for (i =1; i<=rows;i++){
   for(j = 0; j<DIM;j++){
     score = 0;
     score = temp[i-1][prevProc(j,DIM)]+temp[i-1][j]+temp[i-1][nextProc(j,DIM)];
     score = score+temp[i][prevProc(j,DIM)]+temp[i][nextProc(j,DIM)];
     score = score+temp[i+1][prevProc(j,DIM)]+temp[i+1][j]+temp[i+1][nextProc(j,DIM)];// number of neighbors
     // determine if the status of the cell is changed
     if(tmpgrid[(i-1)*DIM+j]==0){
        if (score == 3){
         tmpgrid[(i-1)*DIM+j] = 1;
        } 
     } else { 
       if((score<2)||(score>3)){
         tmpgrid[(i-1)*DIM+j] = 0;
        } 
     }
     }
   }
}
int **alloc_2d_int(int rows, int cols){
 int *data = (int *)malloc(rows*cols*sizeof(int));
 int **array = (int**)malloc(rows*sizeof(int*));
 int i;
 for (i=0;i<rows;i++)
       array[i] = &(data[cols*i]);
 return array;
}

