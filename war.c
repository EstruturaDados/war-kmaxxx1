#include <stdio.h>    // Para printf, scanf, fgets, NULL
#include <stdlib.h>   // Para calloc, free, rand, srand
#include <string.h>   // Para strcpy, strcmp, strcspn
#include <time.h>     // Para time (para srand)

// ============================================================================
// --- Constantes Globais ---
// ============================================================================
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 2 // Agora temos 2 missões
#define MAX_STR_LEN 50
#define BUFFER_SIZE 100
#define VITORIA 1
#define DERROTA 0
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
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio** mapa); // Passagem do endereço do ponteiro

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* const mapa); // const correctness
void exibirMissao(int id_missao);
void limparBufferEntrada(); // Utilitária

// Funções de lógica principal do jogo:
int sortearMissao();
void faseDeAtaque(Territorio* mapa);
void simularAtaque(Territorio* atacante, Territorio* defensor);
int verificarVitoria(const Territorio* const mapa, int id_missao); // const correctness

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================
int main() {
    Territorio* mapa = NULL;
    int id_missao_secreta = 0;
    int escolha = -1;
    int vitoria = DERROTA;

    // 1. Configuração Inicial (Setup):
    srand((unsigned int)time(NULL));
    printf("--- Nível Mestre: Missões e Modularização Total ---\n");

    // Aloca a memória (uso de ponteiros)
    mapa = alocarMapa();
    if (mapa == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o mapa.\n");
        return 1;
    }

    // Inicializa o mapa e sorteia a missão
    inicializarTerritorios(mapa);
    id_missao_secreta = sortearMissao();
    printf("Sua cor de exército: %s.\n", COR_JOGADOR);

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        printf("\n==================== RODADA ATUAL ====================\n");
        exibirMapa(mapa);
        exibirMissao(id_missao_secreta);
        exibirMenuPrincipal();

        printf("Escolha sua ação (0 para sair): ");
        if (scanf("%d", &escolha) != 1) {
            limparBufferEntrada();
            escolha = -1;
        }
        limparBufferEntrada();

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa); // Modifica o mapa
                break;
            case 2:
                vitoria = verificarVitoria(mapa, id_missao_secreta);
                if (vitoria == VITORIA) {
                    printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO E VENCEU O JOGO! ***\n");
                    escolha = SAIR; // Sai do loop após a vitória
                } else {
                    printf("\nSeus objetivos ainda não foram alcançados. Continue lutando!\n");
                }
                break;
            case SAIR:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

        if (escolha != SAIR) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (escolha != SAIR);

    // 3. Limpeza:
    liberarMemoria(&mapa); // Passagem por referência para alterar o ponteiro
    return 0;
}

// ============================================================================
// --- Implementação das Funções ---
// ============================================================================

Territorio* alocarMapa() {
    // Aloca dinamicamente a memória
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

void liberarMemoria(Territorio** mapa) {
    // Libera a memória e define o ponteiro como NULL (uso de ponteiro para ponteiro)
    if (*mapa != NULL) {
        free(*mapa);
        *mapa = NULL;
        printf("\nMemória do mapa liberada.\n");
    }
}

void inicializarTerritorios(Territorio* mapa) {
    // Preenche os dados iniciais dos territórios (Modifica o mapa por referência)
    
    // Territórios do Jogador (Vermelho)
    strcpy(mapa[0].nome, "Porto Seguro");
    strcpy(mapa[0].cor_exercito, COR_JOGADOR);
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Sao Paulo");
    strcpy(mapa[1].cor_exercito, COR_JOGADOR);
    mapa[1].tropas = 3;

    // Territórios Inimigos (Azul e Verde)
    strcpy(mapa[2].nome, "Rio de Janeiro");
    strcpy(mapa[2].cor_exercito, "Azul");
    mapa[2].tropas = 4;

    strcpy(mapa[3].nome, "Minas Gerais");
    strcpy(mapa[3].cor_exercito, "Verde"); // Alvo de uma das missões
    mapa[3].tropas = 3;

    strcpy(mapa[4].nome, "Bahia");
    strcpy(mapa[4].cor_exercito, "Neutro");
    mapa[4].tropas = 2;
}

void limparBufferEntrada() {
    // Utilitário para limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void exibirMenuPrincipal() {
    // Imprime na tela o menu de ações disponíveis
    printf("\n--- Menu de Ações ---\n");
    printf("1. Atacar Território\n");
    printf("2. Verificar Missão/Vitória\n");
    printf("0. Sair do Jogo\n");
}

void exibirMapa(const Territorio* const mapa) {
    // Mostra o estado atual. Usa const para garantir que os dados não mudam.
    printf("\n--- Mapa do Mundo ---\n");
    printf("ID | Nome                    | Dono           | Tropas\n");
    printf("---|-------------------------|----------------|-------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-2d | %-23s | %-14s | %d\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor_exercito,
               mapa[i].tropas);
    }
}

int sortearMissao() {
    // Sorteia e retorna um ID de missão aleatório (1 ou 2)
    return (rand() % NUM_MISSOES) + 1;
}

void exibirMissao(int id_missao) {
    // Exibe a descrição da missão atual
    printf("\n--- Missão Secreta (ID %d) ---\n", id_missao);
    switch (id_missao) {
        case 1:
            printf("Objetivo: Conquistar no mínimo 3 territórios.\n");
            break;
        case 2:
            printf("Objetivo: Destruir completamente o exército VERDE.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
            break;
    }
}

void faseDeAtaque(Territorio* mapa) {
    // Gerencia a interface de ataque
    int id_origem, id_destino;
    int id_origem_idx, id_destino_idx;

    printf("\n--- Iniciar Ataque ---\n");

    printf("ID (1 a %d) do seu território atacante: ", NUM_TERRITORIOS);
    if (scanf("%d", &id_origem) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();
    id_origem_idx = id_origem - 1;

    // Validação do atacante
    if (id_origem_idx < 0 || id_origem_idx >= NUM_TERRITORIOS ||
        strcmp(mapa[id_origem_idx].cor_exercito, COR_JOGADOR) != 0 ||
        mapa[id_origem_idx].tropas <= 1) {
        printf("Ataque inválido: Território não é seu, ID incorreto ou tropas insuficientes (mínimo 2).\n");
        return;
    }

    printf("ID (1 a %d) do território defensor: ", NUM_TERRITORIOS);
    if (scanf("%d", &id_destino) != 1) {
        printf("Entrada inválida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();
    id_destino_idx = id_destino - 1;

    // Validação do defensor
    if (id_destino_idx < 0 || id_destino_idx >= NUM_TERRITORIOS || id_origem == id_destino ||
        strcmp(mapa[id_destino_idx].cor_exercito, COR_JOGADOR) == 0) {
        printf("Ataque inválido: ID incorreto ou você está atacando um território próprio.\n");
        return;
    }

    // Chama a simulação da batalha
    simularAtaque(&mapa[id_origem_idx], &mapa[id_destino_idx]);
}

void simularAtaque(Territorio* atacante, Territorio* defensor) {
    // Executa a lógica da batalha (modifica atacante e defensor por referência - ponteiros)
    int rolagem_atacante = (rand() % 6) + 1;
    int rolagem_defensor = (rand() % 6) + 1;

    printf("\n--- Resultado do Combate ---\n");
    printf("Atacante (%s - %d tropas) rolou: %d\n", atacante->nome, atacante->tropas, rolagem_atacante);
    printf("Defensor (%s - %d tropas) rolou: %d\n", defensor->nome, defensor->tropas, rolagem_defensor);

    if (rolagem_atacante > rolagem_defensor) {
        // Atacante vence
        printf("O ATAQUE foi vitorioso! O defensor perde 1 tropa.\n");
        defensor->tropas--;

        if (defensor->tropas == 0) {
            // Conquista: Território é tomado
            printf("\n*** CONQUISTA! %s tomou %s. ***\n", COR_JOGADOR, defensor->nome);

            // Atualiza dono
            strcpy(defensor->cor_exercito, COR_JOGADOR);

            // Move 1 tropa
            atacante->tropas--;
            defensor->tropas++;
        }
    } else {
        // Defensor vence ou empata
        printf("A DEFESA resistiu. O atacante perde 1 tropa.\n");
        atacante->tropas--;
    }
    
    // Garante que o número de tropas não fique menor que o mínimo de defesa
    if (atacante->tropas < 1) atacante->tropas = 1;
    if (defensor->tropas < 0) defensor->tropas = 0;
}

int verificarVitoria(const Territorio* const mapa, int id_missao) {
    // Verifica se a missão foi cumprida (apenas leitura do mapa)
    int count_territorios_jogador = 0;
    int tropas_verde = 0;

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor_exercito, COR_JOGADOR) == 0) {
            count_territorios_jogador++;
        }
        if (strcmp(mapa[i].cor_exercito, "Verde") == 0) {
            tropas_verde += mapa[i].tropas;
        }
    }

    printf("\n--- Verificação de Missão ---\n");

    switch (id_missao) {
        case 1: // Conquistar 3 territórios
            printf("Status: %d de 3 territórios conquistados.\n", count_territorios_jogador);
            if (count_territorios_jogador >= 3) {
                return VITORIA;
            }
            break;
        case 2: // Destruir o exército VERDE
            printf("Status: O exército VERDE possui %d tropas restantes.\n", tropas_verde);
            if (tropas_verde == 0) {
                return VITORIA;
            }
            break;
    }

    return DERROTA;
}