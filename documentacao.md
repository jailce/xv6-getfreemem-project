# Relatório Técnico: Implementação de Gerenciamento de Memória e Escalonador MLQ no xv6

**Universidade Federal de Uberlândia (UFU)**  
**Disciplina:** Sistemas Operacionais  
**Alunos:** Anna, Jailce Fernanda Brito

---

## 1. Introdução
Este relatório mostra o que a nossa equipe fez para alterar o funcionamento do xv6, focando em duas partes: gerenciar a memória e mudar como os processos são escolhidos para rodar (escalonamento). 

O xv6 original é bem simples. Ele não avisa pro usuário quanta memória tem sobrando e usa a política de Round Robin (onde todo mundo ganha uma quantidade de tempo igual na CPU). O nosso objetivo foi adicionar uma função `getfreemem` para a gente conseguir ver a memória livre, e criar um escalonador de Múltiplas Filas (MLQ) com prioridades, junto com a chamada `setpriority`.

---

## 2. Gerenciamento de Memória: Chamada `getfreemem`
Para a gente conseguir inspecionar quanta memória física está sobrando no sistema, desenvolvemos a chamada `getfreemem`. 

### Como foi feito no Kernel (`kernel/kalloc.c`)
No xv6, a memória livre fica guardada numa lista encadeada chamada `freelist`. A lógica que implementamos basicamente percorre essa lista a partir do começo e conta quantas páginas de 4096 bytes estão disponíveis.

### Mapeamento da Syscall
Para a função existir de verdade e o usuário poder chamar, seguimos o padrão de interrupção do xv6 adicionando as assinaturas nestes arquivos:
* **`kernel/syscall.h`**: Definimos o número da syscall.
* **`kernel/syscall.c`**: Mapeamos a função no vetor do kernel.
* **`user/user.h` e `user/usys.pl`**: Colocamos a assinatura `int getfreemem(void);` para o código de usuário conseguir enxergar a função e gerar o assembly.

Para testar, incluímos um programa chamado `teste_mem.c` no Makefile. Ele chama a função, aloca um pouco de memória e chama de novo para ver se o número de páginas diminuiu corretamente.

---

