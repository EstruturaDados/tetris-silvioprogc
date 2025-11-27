#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// --- CONSTANTES ---
#define QUEUE_CAPACITY 5 // Capacidade real da FILA
#define MAX_QUEUE_SIZE (QUEUE_CAPACITY + 1) // Tamanho do array da fila circular
#define MAX_STACK_SIZE 3 // Capacidade máxima da PILHA de reserva

// --- ESTRUTURA DE DADOS ---

// Struct para representar uma peça de Tetris
typedef struct {
    char nome;     // Tipo da peça ('I', 'O', 'T', 'L', 'J', 'Z', 'S')
    int id;        // Identificador numérico único
} Peca;

// --- VARIÁVEIS GLOBAIS ---

// Fila Circular (FIFO)
Peca fila[MAX_QUEUE_SIZE]; 
int frente = 0;      
int re = 0;          

// Pilha Linear (LIFO)
Peca pilha[MAX_STACK_SIZE];
int topo = -1; // Índice do topo da pilha. -1 indica pilha vazia.

int next_id = 0;     // Contador para gerar IDs únicos.

// --- FUNÇÕES DE UTILIDADE ---

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inicializar_random() {
    srand(time(NULL));
}

// Gera uma nova peça com um tipo aleatório e ID único
Peca gerarNovaPeca() {
    Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    nova_peca.nome = tipos[rand() % num_tipos];
    nova_peca.id = next_id++;
    
    return nova_peca;
}

// --- FUNÇÕES DE CONTROLE DE FILA (Circular Queue) ---

int is_queue_empty() {
    return frente == re;
}

int is_queue_full() {
    return (re + 1) % MAX_QUEUE_SIZE == frente;
}

// Adiciona uma nova peça ao final da fila (Enqueue)
void enqueue(Peca p) {
    if (is_queue_full()) {
        printf("ALERTA: Fila cheia. Nao foi possivel reinserir a peca gerada.\n");
        return;
    }
    fila[re] = p;
    re = (re + 1) % MAX_QUEUE_SIZE;
}

// Remove e retorna a peça da frente da fila (Dequeue)
Peca dequeue() {
    Peca peca_removida = {'X', -1};
    if (!is_queue_empty()) {
        peca_removida = fila[frente];
        frente = (frente + 1) % MAX_QUEUE_SIZE;
    }
    return peca_removida;
}

// Retorna a peça da frente da fila sem remover (Peek)
Peca peek_queue() {
    if (!is_queue_empty()) {
        return fila[frente];
    }
    Peca peca_vazia = {'?', -1};
    return peca_vazia;
}

// --- FUNÇÕES DE CONTROLE DE PILHA (Stack) ---

int is_stack_empty() {
    return topo == -1;
}

int is_stack_full() {
    return topo == MAX_STACK_SIZE - 1;
}

// Adiciona uma peça ao topo da pilha (Push)
int push(Peca p) {
    if (is_stack_full()) {
        return 0; // Falha
    }
    topo++;
    pilha[topo] = p;
    return 1; // Sucesso
}

// Remove e retorna a peça do topo da pilha (Pop)
Peca pop() {
    Peca peca_removida = {'X', -1};
    if (!is_stack_empty()) {
        peca_removida = pilha[topo];
        topo--;
    }
    return peca_removida;
}

// --- FUNÇÕES DE EXIBIÇÃO ---

void exibirFilaEPilha() {
    // Exibe a FILA
    printf("\n------------------------------------------------------------\n");
    printf("ESTADO ATUAL DO JOGO:\n");
    printf("------------------------------------------------------------\n");

    printf("Fila de Peças (FIFO) [Frente -> Ré]: ");
    if (is_queue_empty()) {
        printf("Vazia. (0/%d)\n", QUEUE_CAPACITY);
    } else {
        printf("(%d/%d) [", (re - frente + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE, QUEUE_CAPACITY);
        int i = frente;
        while (i != re) {
            printf("%c %d", fila[i].nome, fila[i].id);
            i = (i + 1) % MAX_QUEUE_SIZE;
            if (i != re) {
                 printf("] [");
            }
        }
        printf("]\n");
    }
    
    // Exibe a PILHA
    printf("Pilha de Reserva (LIFO) (Topo -> Base): ");
    if (is_stack_empty()) {
        printf("Vazia. (0/%d)\n", MAX_STACK_SIZE);
    } else {
        printf("(%d/%d) ", topo + 1, MAX_STACK_SIZE);
        printf("[");
        for (int i = topo; i >= 0; i--) {
            printf("%c %d", pilha[i].nome, pilha[i].id);
            if (i > 0) {
                 printf("] [");
            }
        }
        printf("]\n");
    }
    printf("------------------------------------------------------------\n");
}


// --- FUNÇÕES DE AÇÃO DO JOGADOR ---

// Ação 1: Remove a peça da frente da fila e insere uma nova peça no final.
void jogarPeca() {
    printf("\n--- 1. JOGAR PECA ---\n");
    if (is_queue_empty()) {
        printf("ERRO: Fila de peças vazia. Nenhuma peça para jogar.\n");
        return;
    }
    
    Peca peca_jogada = dequeue();
    printf("SUCESSO: Peça [%c %d] removida da fila e JOGADA!\n", peca_jogada.nome, peca_jogada.id);

    Peca nova_peca = gerarNovaPeca();
    enqueue(nova_peca);
    printf("Fila reabastecida: Peça [%c %d] adicionada ao final.\n", nova_peca.nome, nova_peca.id);
}

// Ação 2: Move a peça da frente da fila para a pilha de reserva.
void reservarPeca() {
    printf("\n--- 2. RESERVAR PECA ---\n");
    
    if (is_stack_full()) {
        printf("ERRO: Pilha de reserva cheia (%d/%d). Use uma peça reservada antes de reservar outra.\n", MAX_STACK_SIZE, MAX_STACK_SIZE);
        return;
    }

    if (is_queue_empty()) {
        printf("ERRO: Fila de peças vazia. Nenhuma peça para reservar.\n");
        return;
    }

    Peca peca_a_reservar = peek_queue();

    if (push(peca_a_reservar)) {
        dequeue(); 
        printf("SUCESSO: Peça [%c %d] movida da fila para a Pilha de Reserva.\n", peca_a_reservar.nome, peca_a_reservar.id);
        
        Peca nova_peca = gerarNovaPeca();
        enqueue(nova_peca);
        printf("Fila reabastecida: Peça [%c %d] adicionada ao final.\n", nova_peca.nome, nova_peca.id);
    }
}

// Ação 3: Remove a peça do topo da pilha de reserva.
void usarPecaReservada() {
    printf("\n--- 3. USAR PECA RESERVADA ---\n");
    if (is_stack_empty()) {
        printf("ERRO: Pilha de reserva vazia. Nenhuma peça reservada para uso.\n");
        return;
    }
    
    Peca peca_usada = pop();
    printf("SUCESSO: Peça [%c %d] removida do topo da pilha e USADA!\n", peca_usada.nome, peca_usada.id);
}


// --- INICIALIZAÇÃO E MENU ---

// Inicializa a fila com 5 peças e a pilha vazia.
void inicializarEstruturas() {
    inicializar_random(); 

    // 1. Inicializa a fila com 5 peças (para replicar [T 0] [O 1] [L 2] [I 3] [I 4])
    printf("Inicializando fila de peças futuras com %d elementos...\n", QUEUE_CAPACITY);
    char tipos_iniciais[] = {'T', 'O', 'L', 'I', 'I'}; 
    int num_elementos = sizeof(tipos_iniciais) / sizeof(tipos_iniciais[0]);
    
    for (int i = 0; i < num_elementos; i++) {
        Peca nova_peca;
        nova_peca.nome = tipos_iniciais[i];
        nova_peca.id = next_id++;
        enqueue(nova_peca);
    }
    
    printf("Pilha de reserva inicializada VAZIA (capacidade %d).\n", MAX_STACK_SIZE);
    
    printf("Estruturas prontas para o jogo.\n");
}


void menu() {
    int opcao;
    
    inicializarEstruturas(); 

    do {
        exibirFilaEPilha();
        
        printf("\nOpções de Ação:\n");
        printf("1. Jogar peça\n");
        printf("2. Reservar peça\n");
        printf("3. Usar peça reservada\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1; 
            limpar_buffer();
        }

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 0:
                printf("\nSaindo do Tetris Stack. Game Over!\n");
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha 1, 2, 3 ou 0.\n");
                break;
        }
    } while (opcao != 0);
}

int main() {
    menu();
    return 0;
}