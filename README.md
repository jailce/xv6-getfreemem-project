**Universidade Federal de Uberlândia**

**Disciplina: GBC045 - Sistemas Operacionais**

**Bacharelado em Ciência da Computação**

---
## Integrantes
Jailce Fernanda Leal dos Santos Brito

Vitor Martins Rocha

Rebecca da Silva Souza

Anna Julia 

Eugênio Marins


## Estrutura
```
Grupo7/
├── README.md
├── slides/
├── video/
├── code/
├── tests/
└── report
```

## Etapa 1 - 09/06/2026


### Vídeo de 5 até 7 minutos apresentando:
- [x] tema escolhido;
- [x] motivação do projeto;
- [x] algoritmos e mecanismos implementados;
- [x] funcionamento da solução;
- [x] proposta de implementação no xv6.

### Requisitos obrigatórios do vídeo
- [x] Todos os integrantes do grupo devem aparecer no vídeo, com participação individual
identificável.
- [x] Cada integrante deverá:
- [x] apresentar parte do conteúdo;
- [x] participar com áudio e vídeo ativos;
- [x] explicar tecnicamente ao menos uma parte do projeto.

## Etapa 2 - 20/07/2026

## Guia de Execução: xv6 - Tema 11 (Memória e MLQ)

Este documento contém o passo a passo rápido para compilar o nosso kernel modificado e executar os testes práticos que comprovam o funcionamento das syscalls e do escalonador de prioridades.

## 1. Compilando e Rodando o QEMU
Para conseguirmos observar o escalonador MLQ agindo de forma estrita (sem que o paralelismo de múltiplos núcleos mascare o resultado), **é fundamental iniciar o xv6 forçando o uso de apenas 1 CPU**.

No seu terminal do Linux, dentro da pasta raiz do `xv6-riscv`, execute:

```bash
make clean
make qemu CPUS=1
2. Testando a Syscall de Memória (getfreemem)
Assim que o xv6 carregar e o shell ($) aparecer, você pode testar a nossa implementação de leitura da memória livre rodando o programa de teste:

Bash
$ teste_mem
(Nota da equipe: o programa vai imprimir a quantidade inicial de bytes livres, forçar a alocação de alguns megabytes para mostrar o valor caindo e, em seguida, acionar o free() para testar o comportamento do heap).

3. Testando o Escalonador MLQ e Prioridades
Para validar que o antigo Round Robin foi substituído pelas nossas 3 filas de prioridade estrita (0, 1 e 2), execute o programa de estresse da CPU:

Bash
$ teste_esc
(Nota da equipe: o teste vai gerar 3 processos filhos e forçar cada um para uma fila diferente usando a nossa syscall setpriority(). Na tela, você verá claramente que o Filho da Fila 0 monopoliza o sistema e termina sua execução inteira antes que a Fila 1 tenha qualquer chance de rodar, provando o funcionamento da prioridade estrita).

4. Como sair do emulador
Para encerrar o xv6 e voltar ao terminal principal do seu sistema, utilize o atalho padrão do QEMU:
Pressione Ctrl + A, solte as teclas, e em seguida aperte X.