#include <stdio.h>    // Para printf, scanf, fgets
#include <string.h>   // Para strcpy

// ============================================================================
// --- Constantes Globais ---
// ============================================================================
#define NUM_TERRITORIOS 5
#define MAX_STR_LEN 50
#define BUFFER_SIZE 100 // Buffer auxiliar para leitura de strings

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
void cadastrarTerritorios(Territorio mapa[]);
void exibirMapa(const Territorio mapa[]);
void limparBufferEntrada(); // Utilitária para limpar o buffer após scanf

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================
int main() {
    // Declaração de um vetor estático de 5 elementos do tipo Territorio.
    Territorio mapa[NUM_TERRITORIOS];

    printf("--- Cadastro Inicial dos Territórios (WAR) ---\n");

    // 1. Cadastro dos territórios
    cadastrarTerritorios(mapa);

    // 2. Exibição do estado atual do mapa
    exibirMapa(mapa);

    return 0;
}

// ============================================================================
// --- Implementação das Funções ---
// ============================================================================

void limparBufferEntrada() {
    // Função utilitária para limpar o buffer de entrada, crucial após scanf.
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void cadastrarTerritorios(Territorio mapa[]) {
    // Realiza a leitura dos dados para cada um dos 5 territórios.
    char buffer[BUFFER_SIZE]; // Buffer temporário para leitura segura de strings

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Território %d de %d ---\n", i + 1, NUM_TERRITORIOS);

        // Leitura do Nome (usando fgets para ler a linha inteira, mais seguro)
        printf("Nome do Território: ");
        // Verifica se fgets conseguiu ler a linha
        if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
            // Remove o '\n' que o fgets adiciona
            buffer[strcspn(buffer, "\n")] = 0;
            // Copia o nome, garantindo que não ultrapasse o tamanho do campo nome
            strncpy(mapa[i].nome, buffer, MAX_STR_LEN - 1);
            mapa[i].nome[MAX_STR_LEN - 1] = '\0'; // Garante terminação nula
        } else {
             // Tratamento de erro (pode ser mais robusto em um código real)
            printf("Erro na leitura do nome. Encerrando.\n");
            return;
        }

        // Leitura da Cor do Exército (usando fgets)
        printf("Cor do Exército Dominante: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;
            strncpy(mapa[i].cor_exercito, buffer, MAX_STR_LEN - 1);
            mapa[i].cor_exercito[MAX_STR_LEN - 1] = '\0';
        } else {
            printf("Erro na leitura da cor. Encerrando.\n");
            return;
        }

        // Leitura do Número de Tropas (usando scanf)
        printf("Número de Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1) {
            printf("Entrada de tropas inválida. Usando 0.\n");
            mapa[i].tropas = 0;
        }
        limparBufferEntrada(); // Limpa o buffer após scanf para o próximo fgets
    }
}

void exibirMapa(const Territorio mapa[]) {
    // Mostra o estado atual de todos os territórios em formato de tabela.
    // Usa 'const' para indicar que esta função não modifica o vetor 'mapa'.

    printf("\n=======================================================\n");
    printf("                  MAPA ATUAL (WAR)\n");
    printf("=======================================================\n");
    printf("ID | Nome                    | Dono           | Tropas\n");
    printf("---|-------------------------|----------------|-------\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-2d | %-23s | %-14s | %d\n",
               i + 1, // ID começando em 1
               mapa[i].nome,
               mapa[i].cor_exercito,
               mapa[i].tropas);
    }
    printf("=======================================================\n");
}