 

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    printf("Iniciando teste da Syscall getfreemem \n");

    // Chama a função count_free_mem() para obter a quantidade de memória livre inicial em bytes
    printf("Contando memória livre...\n");
    uint64 free_mem = count_free_mem(); 

    //Força a gastar memoria
    printf("Alocando memória...\n");
    char *mem1 = malloc(1024 * 1024); // Aloca 1 MB de memória
    char *mem2 = malloc(2 * 1024 * 1024); // Aloca 2 MB de memória                    

    // Imprime a quantidade de memória livre
    printf("Memória livre: %llu bytes\n", free_mem); 

    //Libera a memoria
    printf("Liberando memória...\n");
    free(mem1); // Libera a memória alocada anteriormente
    free(mem2); // Libera a memória alocada anteriormente

    exit(0); 
}

