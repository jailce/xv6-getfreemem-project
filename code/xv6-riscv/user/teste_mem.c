#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	uint64 antes, depois;
	printf("Iniciando teste da Syscall getfreemem\n");
	antes = getfreemem();
	printf("Memoria livre ANTES da alocacao: %lu bytes\n", antes);
	printf("Alocando memoria...\n");
	char *mem1 = malloc(1024 * 1024);
	char *mem2 = malloc(2 * 1024 * 1024);
	depois = getfreemem();
	printf("Memoria livre DEPOIS da alocacao: %lu bytes\n", depois);
	printf("Liberando memoria...\n");
	free(mem1);
	free(mem2);
	exit(0);
}

