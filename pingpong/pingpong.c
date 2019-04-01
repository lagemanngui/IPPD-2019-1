#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  //Número de iterações do ping pong
  const int PING_PONG_LIMIT = 10;

  // Inicializa o MPI, deve ser chamado antes de qualquer outro codigo MPI
  MPI_Init(NULL, NULL);
  
  // Rank (processador) em que esta rodando o codigo atual
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  
  // Numero de processadores rodando para o problema
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Deve ter 2 processos para jogar ping pong
  // caso seja insuficiente, aborta o programa
  if (world_size != 2) {
    fprintf(stderr, "World size must be two for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // Contagem de iterações
  int ping_pong_count = 0;
  
  // Rank do adversario para enviar a mensagem
  // Rank 0 : adversario 1 % 2 = 1
  // Rank 1 : adversario 2 % 2 = 0
  int partner_rank = (world_rank + 1) % 2;
  
  //Loop principal enquanto a contagem nao atingiu o limite
  while (ping_pong_count < PING_PONG_LIMIT) {
  
    //Se for o turno do processador atual envia a bola (mensagem)
    if (world_rank == ping_pong_count % 2) {
      // Incrementa contagem do turno
      ping_pong_count++;
      
      //Envia o valor atual da contagem para o adversario
      MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      
      //Mesagem de debug
      printf("%d sent and incremented ping_pong_count %d to %d\n", 
               world_rank, ping_pong_count, partner_rank);
      
      
    }
    //Se nao for turno do processo atual recebe a mensagem 
    else {
    
      //Recebe e atualiza o valor da varialve ping_pong_count
      MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
               
      // Mensagem de debug
      printf("%d received ping_pong_count %d from %d\n",
             world_rank, ping_pong_count, partner_rank);
    }
  }
  
  //Quando atingit o maximo de loops finaliza o MPI
  MPI_Finalize();
}
