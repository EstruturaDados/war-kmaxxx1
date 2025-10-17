#include <stdio.h>    // Para printf, scanf, fgets, NULL
#include <stdlib.h>   // Para calloc, free, rand, srand
#include <string.h>   // Para strcpy, strcmp, strcspn
#include <time.h>     // Para time (para srand)

// ============================================================================
// --- Constantes Globais ---
// ============================================================================
#define NUM_TERRITORIOS 5
#define MAX_STR_LEN 50
#define BUFFER_SIZE 100 // Buffer auxiliar para leitura de strings
#define SAIR 0

// Cor do jogador (fixa para o exemplo)
const char* COR_JOGADOR = "Vermelho";

// ============================================================================
// --- Estrutura de Dados ---
// ============================================================================
// Define a estrutura para um território.
typedef struct {
    char nome[MAX_STR_LEN];
    char cor_exercito[MAX_STR_LEN]; // Ex: "Vermelho", "Azul"
    int tropas;
} Territorio;

// ============================================================================
// --- Protótipos das Funções ---
// ============================================================================
// Funções de gerenciamento de memória e setup
Territorio* alocarMapa();
void liberarMemoria(Territorio** mapa);
void cadastrarTerritorios(Territorio* mapa);

// Funções de Interface
void exibirMenuPrincipal();
void exibirMapa(const Territorio* const mapa); // const correctness
void limparBufferEntrada(); // Utilitária

// Funções de Lógica de Jogo
void faseDeAtaque(Territorio* mapa);
void simularAtaque(Territorio* atacante, Territorio* defensor);

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================
int main() {
    Territorio* mapa = NULL;
    int escolha = -1;

    // 1. Configuração Inicial
    srand((unsigned int)time(NULL)); // Inicializa a semente para rand()
    printf("--- Nível Aventureiro: Batalhas Estratégicas ---\n");

    // Alocação dinâmica de memória com calloc
    mapa = alocarMapa();
    if (mapa == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o mapa.\n");
        return 1;
    }

    // Cadastro dos territórios
    cadastrarTerritorios(mapa);

    // 2. Laço Interativo (Game Loop Simplificado)
    do {
        printf("\n==================== FASE DE JOGO ====================\n");
        exibirMapa(mapa);
        exibirMenuPrincipal();

        printf("Escolha sua ação (0 para sair): ");
        if (scanf("%d", &escolha) != 1) {
            limparBufferEntrada();
            escolha = -1;
        }
        limparBufferEntrada(); // Limpa o buffer após scanf

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case SAIR:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (escolha != SAIR);

    // 3. Limpeza: Liberar a memória alocada
    liberarMemoria(&mapa);

    return 0;
}

// ============================================================================
// --- Implementação das Funções ---
// ============================================================================

Territorio* alocarMapa() {
    // Aloca dinamicamente a memória para o vetor de territórios usando calloc
    // calloc inicializa a memória com zero/nulo
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

void liberarMemoria(Territorio** mapa) {
    // Libera a memória previamente alocada e define o ponteiro como NULL
    if (*mapa != NULL) {
        free(*mapa);
        *mapa = NULL;
        printf("\nMemória do mapa liberada.\n");
    }
}

void limparBufferEntrada() {
    // Função utilitária para limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void exibirMenuPrincipal() {
    // Imprime na tela o menu de ações disponíveis
    printf("\n--- Menu de Ações ---\n");
    printf("1. Iniciar Ataque\n");
    printf("0. Sair\n");
}

void cadastrarTerritorios(Territorio* mapa) {
    // Versão simplificada do cadastro, apenas inicializando para o jogo poder rodar
    printf("Configurando o mapa inicial...\n");
    
    // Território do Jogador
    strcpy(mapa[0].nome, "Porto Seguro");
    strcpy(mapa[0].cor_exercito, COR_JOGADOR);
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Sao Paulo");
    strcpy(mapa[1].cor_exercito, COR_JOGADOR);
    mapa[1].tropas = 3;

    // Territórios Neutros/Inimigos
    strcpy(mapa[2].nome, "Rio de Janeiro");
    strcpy(mapa[2].cor_exercito, "Azul");
    mapa[2].tropas = 4;

    strcpy(mapa[3].nome, "Minas Gerais");
    strcpy(mapa[3].cor_exercito, "Verde");
    mapa[3].tropas = 3;

    strcpy(mapa[4].nome, "Bahia");
    strcpy(mapa[4].cor_exercito, "Neutro");
    mapa[4].tropas = 2;

    printf("Mapa inicializado com %d territórios. Sua cor é %s.\n", NUM_TERRITORIOS, COR_JOGADOR);
}


void exibirMapa(const Territorio* const mapa) {
    // Mostra o estado atual de todos os territórios em formato de tabela.
    printf("\n--- Mapa do Mundo ---\n");
    printf("ID | Nome                    | Dono           | Tropas\n");
    printf("---|-------------------------|----------------|-------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-2d | %-23s | %-14s | %d\n",
               i + 1, // ID começando em 1
               mapa[i].nome,
               mapa[i].cor_exercito,
               mapa[i].tropas);
    }
}

void faseDeAtaque(Territorio* mapa) {
    // Gerencia a escolha de atacante e defensor pelo jogador
    int id_origem_idx, id_destino_idx;
    int id_origem, id_destino;

    printf("\n--- Iniciar Ataque ---\n");

    printf("ID (1 a %d) do seu território atacante: ", NUM_TERRITORIOS);
    if (scanf("%d", &id_origem) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();
    id_origem_idx = id_origem - 1; // Ajusta para o índice do vetor (0 a 4)

    // Validação básica do atacante
    if (id_origem_idx < 0 || id_origem_idx >= NUM_TERRITORIOS ||
        strcmp(mapa[id_origem_idx].cor_exercito, COR_JOGADOR) != 0) {
        printf("ID de origem inválido ou território não é seu.\n");
        return;
    }
    if (mapa[id_origem_idx].tropas <= 1) {
        printf("Território atacante precisa de pelo menos 2 tropas (1 fica para defesa).\n");
        return;
    }

    printf("ID (1 a %d) do território defensor: ", NUM_TERRITORIOS);
    if (scanf("%d", &id_destino) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();
    id_destino_idx = id_destino - 1; // Ajusta para o índice do vetor

    // Validação básica do defensor
    if (id_destino_idx < 0 || id_destino_idx >= NUM_TERRITORIOS || id_origem == id_destino) {
        printf("ID de destino inválido ou você não pode atacar a si mesmo.\n");
        return;
    }
    if (strcmp(mapa[id_destino_idx].cor_exercito, COR_JOGADOR) == 0) {
        printf("Este território já é seu.\n");
        return;
    }

    // Chama a função de simulação da batalha
    simularAtaque(&mapa[id_origem_idx], &mapa[id_destino_idx]);
}

void simularAtaque(Territorio* atacante, Territorio* defensor) {
    // Simula uma batalha rolando um dado para cada lado (1 a 6)
    int rolagem_atacante = (rand() % 6) + 1;
    int rolagem_defensor = (rand() % 6) + 1;

    printf("\n--- Resultado do Combate ---\n");
    printf("Atacante (%s - %d tropas) rolou: %d\n", atacante->nome, atacante->tropas, rolagem_atacante);
    printf("Defensor (%s - %d tropas) rolou: %d\n", defensor->nome, defensor->tropas, rolagem_defensor);

    // Lógica do Combate
    if (rolagem_atacante > rolagem_defensor) {
        // Atacante vence
        printf("O ATAQUE foi vitorioso! O defensor perde 1 tropa.\n");
        defensor->tropas--;

        if (defensor->tropas == 0) {
            // Conquista: Território é tomado
            printf("\n*** CONQUISTA! %s tomou %s. ***\n", COR_JOGADOR, defensor->nome);

            // Atualiza dono
            strcpy(defensor->cor_exercito, COR_JOGADOR);

            // Move 1 tropa (a tropa vencedora)
            atacante->tropas--;
            defensor->tropas++;

            printf("Uma tropa foi movida de %s para %s.\n", atacante->nome, defensor->nome);
        }
    } else {
        // Defensor vence ou empata (empates favorecem o defensor)
        printf("A DEFESA resistiu ou venceu. O atacante perde 1 tropa.\n");
        atacante->tropas--;
    }
    
    // Prevenção de tropas negativas
    if (atacante->tropas < 1) atacante->tropas = 1;
    if (defensor->tropas < 0) defensor->tropas = 0;
}