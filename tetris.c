#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// --- CONSTANTES ---
#define MAX_SIZE 6 // Capacidade real da fila: MAX_SIZE - 1 = 5

// --- ESTRUTURA DE DADOS ---
typedef struct {
    char nome;     // Tipo da peça ('I', 'O', 'T', 'L', etc.)
    int id;        // Identificador numérico único
} Peca;

// --- VARIÁVEIS GLOBAIS DA FILA ---
Peca fila[MAX_SIZE]; 
int frente = 0;      
int re = 0;          
int next_id = 0;     // Começa em 0

// --- FUNÇÕES DE UTILIDADE ---

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Verifica se a fila está vazia
int is_empty() {
    return frente == re;
}

// Verifica se a fila está cheia
int is_full() {
    return (re + 1) % MAX_SIZE == frente;
}

// MODIFICAÇÃO: Gera uma peça com um tipo fixo ('S') para simplificar o ENQUEUE
Peca gerarNovaPecaParaEnqueue() {
    Peca nova_peca;
    // Tipo fixo 'S' para peças inseridas após a inicialização, 
    // mantendo o foco na lógica da fila.
    nova_peca.nome = 'S'; 
    nova_peca.id = next_id++;
    return nova_peca;
}

// --- OPERAÇÕES DA FILA ---

// Adiciona uma nova peça ao final da fila (Enqueue)
void inserirPeca() {
    printf("\n--- INSERIR NOVA PECA (ENQUEUE) ---\n");
    
    if (is_full()) {
        printf("ERRO: A fila de peças está cheia! Remova uma peça antes de inserir.\n");
        return;
    }
    
    // 1. Gerar a nova peça
    Peca nova_peca = gerarNovaPecaParaEnqueue();
    
    // 2. Inserir a peça na posição 're'
    fila[re] = nova_peca;
    
    // 3. Atualizar o índice 're' (circularmente)
    re = (re + 1) % MAX_SIZE;
    
    printf("SUCESSO: Peça [%c %d] adicionada ao final da fila.\n", nova_peca.nome, nova_peca.id);
}

// Remove a peça da frente da fila (Dequeue)
void jogarPeca() {
    printf("\n--- JOGAR PECA (DEQUEUE) ---\n");

    if (is_empty()) {
        printf("ERRO: A fila de peças está vazia! Nenhuma peça para jogar.\n");
        return;
    }
    
    // 1. Obter a peça da frente
    Peca peca_jogada = fila[frente];
    
    // 2. Atualizar o índice 'frente' (circularmente)
    frente = (frente + 1) % MAX_SIZE;
    
    printf("SUCESSO: Peça [%c %d] removida da frente e JOGADA!\n", peca_jogada.nome, peca_jogada.id);
}

// Exibe o estado atual da fila
void exibirFila() {
    printf("\n--- FILA DE PECAS FUTURAS (Tamanho: %d/%d) ---\n", 
           (re - frente + MAX_SIZE) % MAX_SIZE, 
           MAX_SIZE - 1); 

    if (is_empty()) {
        printf("Fila de peças está vazia.\n");
        return;
    }

    printf("Fila de peças: [");
    
    int i = frente;
    // Percorre a fila de 'frente' até 're' (de forma circular)
    while (i != re) {
        printf("%c %d", fila[i].nome, fila[i].id);
        
        i = (i + 1) % MAX_SIZE;
        if (i != re) {
             printf("] ["); // Formato [T 0] [O 1]
        }
    }
    printf("]\n");
}

// MODIFICAÇÃO: Inicializa a fila com a sequência exata de tipos e IDs
void inicializarFila() {
    // Sequência de tipos desejada: T, O, L, I, I
    char tipos_iniciais[] = {'T', 'O', 'L', 'I', 'I'}; 
    int num_elementos = sizeof(tipos_iniciais) / sizeof(tipos_iniciais[0]);

    printf("Inicializando fila de peças futuras com %d elementos...\n", num_elementos);
    for (int i = 0; i < num_elementos; i++) {
        Peca nova_peca;
        nova_peca.nome = tipos_iniciais[i];
        nova_peca.id = next_id++; // Usa o ID sequencial: 0, 1, 2, 3, 4

        fila[re] = nova_peca;
        re = (re + 1) % MAX_SIZE;
    }
    printf("Fila inicial: [T 0] [O 1] [L 2] [I 3] [I 4]\n");
    printf("Inicialização concluída.\n");
}

// --- FUNÇÃO MENU PRINCIPAL ---

void menu() {
    int opcao;
    
    // Inicializa a fila com a sequência de tipos e IDs solicitada
    inicializarFila(); 

    do {
        exibirFila();
        
        printf("\nOpções de Ação:\n");
        printf("1. Jogar peça (dequeue)\n");
        printf("2. Inserir nova peça (enqueue)\n");
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
                inserirPeca();
                break;
            case 0:
                printf("\nSaindo do Tetris Stack. Game Over!\n");
                break;
            default:
                printf("\nOpção inválida. Por favor, escolha 1, 2 ou 0.\n");
                break;
        }
    } while (opcao != 0);
}

int main() {
    // Inicializa o gerador de números aleatórios, embora não seja mais usado para o tipo, 
    // é bom manter para futuras expansões.
    srand(time(NULL)); 
    menu();
    return 0;
}