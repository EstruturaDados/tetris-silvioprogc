#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// --- CONSTANTES ---
#define QUEUE_CAPACITY 5 // Capacidade real da FILA
#define MAX_QUEUE_SIZE (QUEUE_CAPACITY + 1) // Tamanho do array da fila circular
#define MAX_STACK_SIZE 3 // Capacidade máxima da PILHA de reserva
#define NUM_TIPOS_SIMPLES 4 // I, O, T, L

// --- ESTRUTURA DE DADOS ---
typedef struct {
    char nome;     // Tipo da peça ('I', 'O', 'T', 'L')
    int id;        // Identificador numérico único
} Peca;

// --- VARIÁVEIS GLOBAIS ---
// Fila Circular (FIFO)
Peca fila[MAX_QUEUE_SIZE]; 
int frente = 0;      
int re = 0;          

// Pilha Linear (LIFO)
Peca pilha[MAX_STACK_SIZE];
int topo = -1; 

int next_id = 0;     // Contador para gerar IDs únicos.

// --- FUNÇÕES DE UTILIDADE ---

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inicializar_random() {
    srand(time(NULL));
}

// Gera uma nova peça com um tipo aleatório ('I', 'O', 'T', 'L') e ID único
Peca gerarNovaPeca() {
    Peca nova_peca;
    char tipos[] = {'I', 'O', 'T', 'L'}; // Apenas estes tipos, conforme requisitado
    
    nova_peca.nome = tipos[rand() % NUM_TIPOS_SIMPLES];
    nova_peca.id = next_id++;
    
    return nova_peca;
}

// Troca os valores de duas peças
void trocarPecas(Peca *p1, Peca *p2) {
    Peca temp = *p1;
    *p1 = *p2;
    *p2 = temp;
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
Peca *peek_queue() {
    if (!is_queue_empty()) {
        return &fila[frente];
    }
    return NULL;
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

// Retorna a peça do topo da pilha sem remover (Peek)
Peca *peek_stack() {
    if (!is_stack_empty()) {
        return &pilha[topo];
    }
    return NULL;
}

// --- FUNÇÕES DE EXIBIÇÃO ---

void exibirFilaEPilha() {
    // Exibe a FILA
    printf("\n--------------------------------------------------------------------------\n");
    printf("ESTADO ATUAL DO JOGO:\n");
    printf("--------------------------------------------------------------------------\n");

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
    printf("--------------------------------------------------------------------------\n");
}


// --- FUNÇÕES DE AÇÃO DO JOGADOR ---

// Ação 1: Jogar Peça (Dequeue + Enqueue)
void jogarPeca() {
    printf("\n--- 1. JOGAR PECA ---\n");
    if (is_queue_empty()) {
        printf("ERRO: Fila de peças vazia. Nenhuma peça para jogar.\n");
        return;
    }
    
    Peca peca_jogada = dequeue();
    printf("SUCESSO: Peça [%c %d] removida da fila e JOGADA!\n", peca_jogada.nome, peca_jogada.id);

    // Reabastece a fila
    Peca nova_peca = gerarNovaPeca();
    enqueue(nova_peca);
    printf("Fila reabastecida: Peça [%c %d] adicionada ao final.\n", nova_peca.nome, nova_peca.id);
}

// Ação 2: Reservar Peça (Dequeue -> Push + Enqueue)
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

    Peca peca_a_reservar = *peek_queue(); // Pega o valor da peça
    
    if (push(peca_a_reservar)) {
        dequeue(); // Remove da fila
        printf("SUCESSO: Peça [%c %d] movida da fila para a Pilha de Reserva.\n", peca_a_reservar.nome, peca_a_reservar.id);
        
        // Reabastece a fila
        Peca nova_peca = gerarNovaPeca();
        enqueue(nova_peca);
        printf("Fila reabastecida: Peça [%c %d] adicionada ao final.\n", nova_peca.nome, nova_peca.id);
    }
}

// Ação 3: Usar Peça Reservada (Pop)
void usarPecaReservada() {
    printf("\n--- 3. USAR PECA RESERVADA ---\n");
    if (is_stack_empty()) {
        printf("ERRO: Pilha de reserva vazia. Nenhuma peça reservada para uso.\n");
        return;
    }
    
    Peca peca_usada = pop();
    printf("SUCESSO: Peça [%c %d] removida do topo da pilha e USADA!\n", peca_usada.nome, peca_usada.id);
    // Nota: Nenhuma nova peça é gerada, a fila não é afetada.
}

// Ação 4: Trocar peça da frente da fila com o topo da pilha (Troca Simples)
void trocarPecaAtual() {
    printf("\n--- 4. TROCAR PECA ATUAL (Fila[0] <-> Pilha[Topo]) ---\n");
    
    if (is_queue_empty()) {
        printf("ERRO: Fila de peças vazia. Troca impossível.\n");
        return;
    }
    if (is_stack_empty()) {
        printf("ERRO: Pilha de reserva vazia. Troca impossível.\n");
        return;
    }

    Peca *peca_fila = peek_queue();
    Peca *peca_pilha = peek_stack();

    printf("Troca realizada: Fila: [%c %d] <-> Pilha: [%c %d]\n", 
           peca_fila->nome, peca_fila->id, peca_pilha->nome, peca_pilha->id);
           
    trocarPecas(peca_fila, peca_pilha);
    printf("SUCESSO: Peças trocadas.\n");
}

// Ação 5: Troca múltipla: 3 primeiras peças da fila com as 3 peças da pilha
void trocarMultipla() {
    printf("\n--- 5. TROCA MULTIPLA (Fila[0..2] <-> Pilha[0..2]) ---\n");
    
    // 1. Verificar se a pilha está cheia (3 peças)
    if (topo != MAX_STACK_SIZE - 1) {
        printf("ERRO: A troca múltipla requer que a PILHA esteja CHEIA (%d peças).\n", MAX_STACK_SIZE);
        return;
    }
    
    // 2. Verificar se a fila tem pelo menos 3 peças. Como a fila é mantida cheia (5 peças), 
    // basta garantir que não esteja quase vazia.
    int tamanho_fila = (re - frente + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    if (tamanho_fila < MAX_STACK_SIZE) {
        printf("ERRO: A troca múltipla requer que a FILA tenha pelo menos %d peças. (Atual: %d)\n", MAX_STACK_SIZE, tamanho_fila);
        return;
    }

    printf("Troca realizada entre os %d primeiros da fila e os %d da pilha.\n", MAX_STACK_SIZE, MAX_STACK_SIZE);
    
    // Realiza a troca dos 3 elementos
    for (int i = 0; i < MAX_STACK_SIZE; i++) {
        // A peça na pilha é indexada [0] (Base), [1], [2] (Topo)
        // A peça na fila é indexada [frente] + i (Circular)
        int indice_fila = (frente + i) % MAX_QUEUE_SIZE;
        
        // Troca o elemento i da pilha com o elemento i da frente da fila
        trocarPecas(&pilha[i], &fila[indice_fila]);
    }
    
    printf("SUCESSO: Troca em bloco concluída.\n");
}

// --- INICIALIZAÇÃO E MENU ---

// Inicializa a fila e a pilha conforme o exemplo.
void inicializarEstruturas() {
    inicializar_random(); 

    // 1. Inicializa a fila com 5 peças (Exemplo: [I 0] [L 1] [T 2] [O 3] [I 4])
    printf("Inicializando fila de peças futuras (%d elementos) e pilha (%d elementos).\n", QUEUE_CAPACITY, MAX_STACK_SIZE);
    
    // Tipos e IDs sequenciais para a fila (0 a 4)
    char tipos_fila[] = {'I', 'L', 'T', 'O', 'I'}; 
    for (int i = 0; i < QUEUE_CAPACITY; i++) {
        Peca nova_peca;
        nova_peca.nome = tipos_fila[i];
        nova_peca.id = next_id++;
        enqueue(nova_peca);
    }
    
    // 2. Inicializa a pilha com 3 peças (Exemplo: [O 8] [L 7] [T 6] - Topo -> Base)

    char tipos_pilha[] = {'T', 'L', 'O'}; // Base -> Topo (T 6, L 7, O 8)
    
    // Ajustar next_id para iniciar os IDs da pilha onde a fila parou (id=5)
   
    next_id = 5; 
    
    // Peças da pilha (Base -> Topo)
    Peca p_base = {tipos_pilha[0], next_id++}; // T 5 (Base)
    Peca p_meio = {tipos_pilha[1], next_id++}; // L 6
    Peca p_topo = {tipos_pilha[2], next_id++}; // O 7 (Topo)

    // Ajuste para replicar o ID do exemplo: [T 6] [L 7] [O 8]
  
    next_id = 6;
    p_base.id = next_id++; // T 6
    p_meio.id = next_id++; // L 7
    p_topo.id = next_id++; // O 8

    push(p_base);
    push(p_meio);
    push(p_topo);
    
    printf("Estruturas prontas. Próxima peça gerada terá ID: %d.\n", next_id);
}


void menu() {
    int opcao;
    
    // Inicializa a fila e a pilha com o estado do exemplo
    inicializarEstruturas(); 

    do {
        exibirFilaEPilha();
        
        printf("\nOpções de Ação:\n");
        printf("1. Jogar peça\n");
        printf("2. Reservar peça\n");
        printf("3. Usar peça reservada\n");
        printf("4. Trocar peça da frente da fila com o topo da pilha (Troca Simples)\n");
        printf("5. Troca Múltipla (3 primeiros da Fila <-> 3 da Pilha)\n");
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
            case 4:
                trocarPecaAtual();
                break;
            case 5:
                trocarMultipla();
                break;
            case 0:
                printf("\nSaindo do Tetris Stack. Game Over!\n");
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha de 0 a 5.\n");
                break;
        }
    } while (opcao != 0);
}

int main() {
    menu();
    return 0;
}