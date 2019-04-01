#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

   MPI_Init(NULL, NULL);
   
   //Pega o rank do processo atual
   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
   //Total de processos
   int size;
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   //Não permite jogar com um processo
   if(size <= 1) {
      printf("Não é possivel jogar anel sozinho");
      MPI_Abort(MPI_COMM_WORLD, 1);
   }
   
   // Iteração atual
   int iteracao = 0;
   
   int destino = rank + 1;
   if(destino == size) destino = 0; //Se for o último volta pro inicio
   
   int origem = rank - 1;
   if(origem < 0) origem = size - 1;
   
   int anel = 0;
   
   printf("Rank %d - Destino %d\n", rank, destino);
   printf("Rank %d - Origem %d\n", rank, origem);
   
   if(rank == 0) {
      printf("Total de processos: %d\n", size);
      anel = -1;
   }
      
   
   //Se tem o anel passa adiante
   if(anel == -1) {
      
      MPI_Send(&anel, 1, MPI_INT, destino, 0, MPI_COMM_WORLD);
      
      //Remove o anel do atual
      anel = 0;
   }
   else{
   
      MPI_Recv(&anel, 1, MPI_INT, origem, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
      printf("%d recebeu o anel de %d", rank, origem);
      
   }

   MPI_Finalize(); 
   
}
