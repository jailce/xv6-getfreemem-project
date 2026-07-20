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

## 3. Escalonador de Múltiplas Filas (MLQ)
A mudança mais importante foi tirar o Round Robin padrão e colocar um sistema com 3 filas de prioridade (0, 1 e 2). A fila 0 é a mais alta e a 2 é a mais baixa.

### Estrutura do Processo (`kernel/proc.h` e `kernel/proc.c`)
Primeiro, tivemos que ensinar o processo a ter uma prioridade. Adicionamos o campo `priority` na estrutura dele:

```c
// Per-process state
struct proc {
  struct spinlock lock;

  enum procstate state;        // Estado do processo (UNUSED, SLEEPING, RUNNABLE, etc)
  void *chan;                  // Canal de suspensão
  int killed;                  // Sinalizador de encerramento
  int xstate;                  // Status de saída retornado ao pai
  int pid;                     // Identificador do Processo (PID)
  int priority;                // Nível de prioridade para o MLQ (0 = Alta, 1 = Média, 2 = Baixa)
  
  // ... demais campos originais
};
```

Depois, no `kernel/proc.c` (dentro do `allocproc`), garantimos que todo processo novo vai nascer com a prioridade 1 (média):
```c
  p->pid = allocpid();
  p->priority = 1;  // Todo processo nasce na prioridade média/padrão
```

### Mudando a função do Escalonador
No xv6 normal, a função `scheduler(void)` verifica a lista de processos inteira e roda quem estiver pronto. Nós mudamos isso colocando um loop externo. 

Agora, o kernel varre a tabela procurando só processos da fila 0. Se achar, ele roda e o loop volta pro começo. Ele só vai olhar as filas 1 e 2 se não tiver ninguém na fila de cima.

```c
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();

  c->proc = 0;
  for (;;) {
    intr_on();
    intr_off();

    int found = 0;

    // Tentativa de criar o MLQ: vamos varrer da fila 0 (maior) ate a fila 2 (menor)
    for (int target_priority = 0; target_priority <= 2; target_priority++) {
      for (p = proc; p < &proc[NPROC]; p++) {
        acquire(&p->lock);
        
        // Verifica se o processo quer rodar e se pertence a fila que estamos olhando agora
        if (p->state == RUNNABLE && p->priority == target_priority) {
          p->state = RUNNING;
          c->proc = p;
          swtch(&c->context, &p->context);

          c->proc = 0;
          found = 1; // Achamos um processo para rodar nessa fila
        }
        release(&p->lock);
      }
      
      // Se rodou alguem na fila alta, quebra para voltar a checar do topo (fila 0)
      if (found) {
        break;
      }
    }

    if (found == 0) {
      asm volatile("wfi");
    }
  }
}
```

---

## 4. Chamada `setpriority`
Para a gente poder trocar a fila de um processo enquanto ele está rodando, criamos a chamada `setpriority(int)`.

### Lógica no Kernel (`kernel/sysproc.c`)
A função resgata o valor digitado pelo usuário, checa se a fila realmente existe (0, 1 ou 2) para não quebrar o sistema, e então muda o valor dentro da estrutura do processo atual. O mapeamento dessa função seguiu os mesmos passos da `getfreemem`.

```c
uint64
sys_setpriority(void)
{
  int priority;

  // Pegando o primeiro argumento inteiro passado pelo usuario
  argint(0, &priority);

  // Validacao basica para evitar que o usuario configure uma fila que nao existe
  if(priority < 0 || priority > 2)
    return -1;

  // Modifica a prioridade do processo atual que chamou a syscall
  struct proc *p = myproc();
  acquire(&p->lock);
  p->priority = priority;
  release(&p->lock);

  return 0;
}
```

---

## 5. Resultados e Comparação
Para testar se tudo isso funcionou na prática, criamos o programa `user/teste_esc.c`. O código cria 3 processos filhos. Cada filho usa o `setpriority` para se jogar numa fila diferente (Fila 0, Fila 1 e Fila 2) e depois entra num loop fazendo milhares de somas com números inteiros para gastar a CPU de propósito.

Rodamos o simulador forçando o uso de apenas uma CPU (`make qemu CPUS=1`) para os resultados ficarem claros. A saída do terminal foi essa:

![Evidência do resultado](image.png)

### Analisando o resultado

---

## 6. Conclusão
