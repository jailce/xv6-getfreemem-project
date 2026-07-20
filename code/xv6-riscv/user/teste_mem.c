#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	// Teste da Syscall getfreemem
	uint64 antes, depois, apos_liberacao;
	printf("Iniciando teste da Syscall getfreemem\n");

	antes = getfreemem();
	// Força a alocação de memória para testar a liberação
	printf("Memoria livre ANTES da alocacao: %lu bytes\n", antes);
	printf("Alocando memoria...\n");
	char *mem1 = malloc(1024 * 1024);
	char *mem2 = malloc(2 * 1024 * 1024);

	depois = getfreemem();
	// Teste da liberacao de memoria
	printf("Memoria livre DEPOIS da alocacao: %lu bytes\n", depois);
	printf("Liberando memoria...\n");
	free(mem1);
	free(mem2);
	// Verifica a quantidade de memória livre após a liberação
	apos_liberacao = getfreemem();
    printf("Memoria livre APOS a liberacao: %lu bytes\n", apos_liberacao);
	
	exit(0);
}

