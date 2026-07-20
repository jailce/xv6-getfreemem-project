#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
loop_calculo()
{
  // Usando inteiros para evitar instrução ilegal de ponto flutuante (scause 0x2)
  volatile int x = 0;
  for (int i = 0; i < 50000000; i++) {
    x += i * 2;
  }
}

int
main(int argc, char *argv[])
{
  printf("Iniciando teste do Escalonador MLQ\n");

  // Criando 3 processos filhos com prioridades distintas
  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("Erro no fork\n");
      exit(1);
    }

    if (pid == 0) {
      // Processo Filho: Fila 0 (Alta), Fila 1 (Média), Fila 2 (Baixa)
      int prioridade_alvo = i; 
      setpriority(prioridade_alvo);
      
      printf("Filho %d iniciado na Fila %d\n", getpid(), prioridade_alvo);
      
      loop_calculo();
      
      printf("Filho %d (Fila %d) TERMINOU\n", getpid(), prioridade_alvo);
      exit(0);
    }
  }

  // Processo Pai: Espera todos os filhos terminarem
  for (int i = 0; i < 3; i++) {
    wait(0);
  }

  printf("Teste do escalonador concluido.\n");
  exit(0);
}