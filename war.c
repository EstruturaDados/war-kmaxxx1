#include <stdio.h>    // Para printf, scanf, getchar
#include <stdlib.h>   // Para malloc, free, srand, rand, NULL
#include <time.h>     // Para time
#include <string.h>   // Para strcpy, strcmp

// ============================================================================
// --- Constantes Globais ---
// ============================================================================
#define NUM_TERRITORIOS 5
#define NUM_MISSOES 3
#define MAX_STR_LEN 50
#define VITORIA 1
#define DERROTA 0
#define SAIR 0

// ============================================================================
// --- Estrutura de Dados ---
// ============================================================================
typedef struct {
    char nome[MAX_STR_LEN];
    char cor_exercito[MAX_STR_LEN]; // "Vermelho", "Azul", "Neutro"
    int tropas;
} Territorio;

// ============================================================================
// --- Protótipos das Funções ---
// ============================================================================

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa, const char* cor_jogador);
void liberarMemoria(Territorio** mapa); // Recebe o endereço do ponteiro para NULL após free

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* const mapa); // const correctness: não modifica o mapa
void exibirMissao(int id_missao);
void limparBufferEntrada(); // Utilitária

// Funções de lógica principal do jogo:
int sortearMissao();
void faseDeAtaque(Territorio* mapa, const char* cor_jogador);
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* cor_jogador);
int verificarVitoria(const Territorio* const mapa, int id_missao, const char* cor_jogador);

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================
int main() {
    // Variáveis de estado
    Territorio* mapa = NULL;
    char cor_jogador[] = "Vermelho"; // Cor fixa do jogador para este exemplo
    int id_missao_secreta = 0;
    int escolha = -1;
    int vitoria = DERROTA;

    // 1. Configuração Inicial (Setup):
    // Define o locale (não estritamente necessário em C++ moderno, mas boa prática em C)
    // Inicializa a semente para geração de números aleatórios
    srand((unsigned int)time(NULL));

    printf("Iniciando o Jogo Estruturado de WAR...\n");

    // Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    mapa = alocarMapa();
    if (mapa == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o mapa.\n");
        return 1;
    }

    // Preenche os territórios com seus dados iniciais.
    inicializarTerritorios(mapa, cor_jogador);

    // Sorteia a missão secreta do jogador.
    id_missao_secreta = sortearMissao();
    printf("Sua cor de exército: %s.\n", cor_jogador);

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        printf("\n==================== RODADA ATUAL ====================\n");
        exibirMapa(mapa);
        exibirMissao(id_missao_secreta);
        exibirMenuPrincipal();

        printf("Escolha sua ação (0 para sair): ");
        if (scanf("%d", &escolha) != 1) {
            limparBufferEntrada();
            escolha = -1; // Garante loop caso a entrada seja inválida
        }
        limparBufferEntrada();

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, cor_jogador);
                break;
            case 2:
                vitoria = verificarVitoria(mapa, id_missao_secreta, cor_jogador);
                if (vitoria) {
                    printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO! ***\n");
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
            printf("\nPressione ENTER para continuar a próxima rodada...");
            getchar(); // Pausa a execução
        }

    } while (escolha != SAIR);

    // 3. Limpeza:
    liberarMemoria(&mapa);

    return 0;
}

// ============================================================================
// --- Implementação das Funções ---
// ============================================================================

Territorio* alocarMapa() {
    // Aloca dinamicamente a memória para o vetor de territórios usando calloc
    // Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

void inicializarTerritorios(Territorio* mapa, const char* cor_jogador) {
    // Preenche os dados iniciais de cada território no mapa
    // Utiliza passagem por referência (ponteiro) para modificar o mapa.
    
    // Configurações iniciais
    strcpy(mapa[0].nome, "Asia Central");
    strcpy(mapa[0].cor_exercito, cor_jogador);
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Groelandia");
    strcpy(mapa[1].cor_exercito, cor_jogador);
    mapa[1].tropas = 3;

    strcpy(mapa[2].nome, "Alaska");
    strcpy(mapa[2].cor_exercito, "Azul");
    mapa[2].tropas = 4;

    strcpy(mapa[3].nome, "Egipto");
    strcpy(mapa[3].cor_exercito, "Azul");
    mapa[3].tropas = 3;

    strcpy(mapa[4].nome, "Madagascar");
    strcpy(mapa[4].cor_exercito, "Neutro");
    mapa[4].tropas = 2;

    printf("Mapa inicializado com %d territórios.\n", NUM_TERRITORIOS);
}

void liberarMemoria(Territorio** mapa) {
    // Libera a memória previamente alocada e define o ponteiro como NULL
    if (*mapa != NULL) {
        free(*mapa);
        *mapa = NULL; // Evita ponteiros pendurados (dangling pointers)
        printf("\nMemória do mapa liberada.\n");
    }
}

void exibirMenuPrincipal() {
    // Imprime na tela o menu de ações disponíveis
    printf("\n--- Menu de Ações ---\n");
    printf("1. Atacar Território\n");
    printf("2. Verificar Missão/Vitória\n");
    printf("0. Sair do Jogo\n");
}

void exibirMapa(const Territorio* const mapa) {
    // Mostra o estado atual de todos os territórios
    // Usa 'const' para garantir que a função apenas leia os dados
    printf("\n--- Mapa do Mundo ---\n");
    printf("ID | Nome                   | Dono           | Tropas\n");
    printf("---|------------------------|----------------|-------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-2d | %-22s | %-14s | %d\n",
               i, mapa[i].nome, mapa[i].cor_exercito, mapa[i].tropas);
    }
}

void exibirMissao(int id_missao) {
    // Exibe a descrição da missão atual do jogador
    printf("\n--- Missão Secreta (ID %d) ---\n", id_missao);
    switch (id_missao) {
        case 1:
            printf("Objetivo: Conquistar no mínimo 4 territórios.\n");
            break;
        case 2:
            printf("Objetivo: Destruir o exército AZUL completamente.\n");
            break;
        case 3:
            printf("Objetivo: Possuir mais de 10 tropas em um único território.\n");
            break;
        default:
            printf("Missão desconhecida.\n");
            break;
    }
}

void limparBufferEntrada() {
    // Função utilitária para limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int sortearMissao() {
    // Sorteia e retorna um ID de missão aleatório
    return (rand() % NUM_MISSOES) + 1; // De 1 a NUM_MISSOES
}

void faseDeAtaque(Territorio* mapa, const char* cor_jogador) {
    // Gerencia a interface para a ação de ataque
    int id_origem, id_destino;

    printf("\n--- Fase de Ataque ---\n");

    printf("ID do seu território de ataque: ");
    if (scanf("%d", &id_origem) != 1 || id_origem < 0 || id_origem >= NUM_TERRITORIOS) {
        printf("ID de origem inválido.\n");
        return;
    }
    limparBufferEntrada();

    // Validação de Dono e Tropas
    if (strcmp(mapa[id_origem].cor_exercito, cor_jogador) != 0) {
        printf("Você não é o dono do território de origem.\n");
        return;
    }
    if (mapa[id_origem].tropas <= 1) {
        printf("O território de origem precisa de pelo menos 2 tropas para atacar (1 de defesa).\n");
        return;
    }

    printf("ID do território a ser atacado: ");
    if (scanf("%d", &id_destino) != 1 || id_destino < 0 || id_destino >= NUM_TERRITORIOS) {
        printf("ID de destino inválido.\n");
        return;
    }
    limparBufferEntrada();

    // Validação de Não-Ataque a Si Próprio
    if (id_origem == id_destino) {
        printf("Não é possível atacar seu próprio território.\n");
        return;
    }

    // Validação de Dono
    if (strcmp(mapa[id_destino].cor_exercito, cor_jogador) == 0) {
        printf("O território de destino já é seu.\n");
        return;
    }
    
    // Executa a lógica de ataque
    simularAtaque(&mapa[id_origem], &mapa[id_destino], cor_jogador);
}

void simularAtaque(Territorio* atacante, Territorio* defensor, const char* cor_jogador) {
    // Executa a lógica de uma batalha
    int rolagem_atacante = (rand() % 6) + 1; // Dado de 1 a 6
    int rolagem_defensor = (rand() % 6) + 1;

    printf("\n--- Resultado do Combate ---\n");
    printf("Atacante (%s - %d tropas) rolou: %d\n", atacante->nome, atacante->tropas, rolagem_atacante);
    printf("Defensor (%s - %d tropas) rolou: %d\n", defensor->nome, defensor->tropas, rolagem_defensor);

    if (rolagem_atacante > rolagem_defensor) {
        // Atacante vence
        printf("O ATAQUE foi bem-sucedido! Uma tropa do defensor é perdida.\n");
        defensor->tropas--;
        
        if (defensor->tropas == 0) {
            // Conquista de Território
            printf("\n>>> CONQUISTA! %s tomou %s.\n", cor_jogador, defensor->nome);
            
            // Atualiza dono
            strcpy(defensor->cor_exercito, cor_jogador);
            
            // Move uma tropa
            atacante->tropas--;
            defensor->tropas++;
            
            printf("Uma tropa foi movida de %s para %s.\n", atacante->nome, defensor->nome);
        }
    } else {
        // Defensor vence ou empata
        printf("A DEFESA resistiu. Uma tropa do atacante é perdida.\n");
        atacante->tropas--;
    }
}

int verificarVitoria(const Territorio* const mapa, int id_missao, const char* cor_jogador) {
    // Verifica se o jogador cumpriu os requisitos de sua missão atual
    int count_territorios_jogador = 0;
    int tropas_azuis = 0;
    int max_tropas = 0;

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        // Conta territórios do jogador e verifica a maior concentração de tropas
        if (strcmp(mapa[i].cor_exercito, cor_jogador) == 0) {
            count_territorios_jogador++;
            if (mapa[i].tropas > max_tropas) {
                max_tropas = mapa[i].tropas;
            }
        }
        
        // Conta tropas do exército AZUL
        if (strcmp(mapa[i].cor_exercito, "Azul") == 0) {
            tropas_azuis += mapa[i].tropas;
        }
    }

    printf("\n--- Verificação de Missão ---\n");

    switch (id_missao) {
        case 1: // Conquistar no mínimo 4 territórios
            printf("Status: %d de 4 territórios conquistados.\n", count_territorios_jogador);
            if (count_territorios_jogador >= 4) {
                return VITORIA;
            }
            break;
        case 2: // Destruir o exército AZUL
            printf("Status: O exército AZUL possui %d tropas restantes.\n", tropas_azuis);
            if (tropas_azuis == 0) {
                return VITORIA;
            }
            break;
        case 3: // Possuir mais de 10 tropas em um único território
            printf("Status: Máxima concentração de tropas é %d (necessário 11+).\n", max_tropas);
            if (max_tropas > 10) {
                return VITORIA;
            }
            break;
    }

    return DERROTA;
}