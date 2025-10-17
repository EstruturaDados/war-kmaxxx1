#include <iostream>
#include <cstdlib>   // calloc, free, rand, srand
#include <cstring>   // strcpy, strlen
#include <ctime>     // time
#include <clocale>   // setlocale
#include <limits>    // numeric_limits

// --- Constantes Globais ---
const int NUM_TERRITORIOS = 8;
const int NUM_MISSOES     = 2;
const int MAX_NOME        = 32;

// Cores/donos (inteiro simples)
const int COR_NEUTRA  = 0;
const int COR_JOGADOR = 1;
const int COR_VERMELHA = 2;
const int COR_VERDE   = 3;

// --- Estrutura de Dados ---
struct Territorio {
    char nome[MAX_NOME];
    int corDoExercito; // dona do território (0 neutro, 1 jogador, 2 vermelho, ...)
    int tropas;
};

// --- Protótipos das Funções ---
// Setup e gerenciamento de memória
Territorio* alocarMapa(int quantidade);
void liberarMemoria(Territorio* mapa);

// Inicialização e exibição
void inicializarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(const Territorio* mapa, int quantidade);

// Interface / menu
void exibirMenuPrincipal();
void exibirMissao(int missaoId, int corJogador);

// Fase de ataque / batalha
void faseDeAtaque(Territorio* mapa, int quantidade, int corJogador);
void simularAtaque(Territorio* origem, Territorio* destino, int corJogador);

// Missões e verificação de vitória
int sortearMissao();
bool verificarVitoria(const Territorio* mapa, int quantidade, int missaoId, int corJogador);

// Utilitárias
void limparBufferEntrada();
int lerInteiroSeguro();

// ---------------------
// --- Função main() ---
// ---------------------
int main() {
    setlocale(LC_ALL, "");              // portugues/locale (se suportado)
    srand(static_cast<unsigned>(time(nullptr)));

    // 1. Configuração inicial
    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (mapa == nullptr) {
        std::cerr << "Erro: falha ao alocar memoria para o mapa.\n";
        return 1;
    }
    inicializarTerritorios(mapa, NUM_TERRITORIOS);

    int corJogador = COR_JOGADOR; // cor do jogador fixa aqui
    int missao = sortearMissao();

    // 2. Laço principal do jogo
    int escolha = -1;
    bool venceu = false;
    do {
        std::cout << "\n=== MAPA ATUAL ===\n";
        exibirMapa(mapa, NUM_TERRITORIOS);
        std::cout << "\n=== MISSÃO ===\n";
        exibirMissao(missao, corJogador);

        exibirMenuPrincipal();
        std::cout << "Escolha: ";
        escolha = lerInteiroSeguro();

        switch (escolha) {
            case 1:
                faseDeAtaque(mapa, NUM_TERRITORIOS, corJogador);
                break;
            case 2:
                if (verificarVitoria(mapa, NUM_TERRITORIOS, missao, corJogador)) {
                    std::cout << "\nParabens! Voce cumpriu a missao e venceu o jogo!\n";
                    venceu = true;
                } else {
                    std::cout << "\nMissao ainda nao cumprida. Continue tentando!\n";
                }
                break;
            case 0:
                std::cout << "Saindo do jogo...\n";
                break;
            default:
                std::cout << "Opcao invalida. Tente novamente.\n";
        }

    } while (escolha != 0 && !venceu);

    // 3. Limpeza
    liberarMemoria(mapa);
    return 0;
}

// -----------------------------
// --- Implementacao das funcoes
// -----------------------------

// Aloca dinamicamente o mapa (calloc conforme pedido)
Territorio* alocarMapa(int quantidade) {
    Territorio* mapa = static_cast<Territorio*>(calloc(quantidade, sizeof(Territorio)));
    return mapa;
}

// Inicializa nomes, donos e tropas iniciais
void inicializarTerritorios(Territorio* mapa, int quantidade) {
    if (mapa == nullptr) return;

    // nomes e configuração inicial (exemplo simples)
    const char* nomesPadrao[NUM_TERRITORIOS] = {
        "Ilha do Norte", "Vila Antiga", "Planalto", "Bosque",
        "Fortaleza", "Ponte Velha", "Deserto", "Vale Azul"
    };
    // Distribuicao inicial de donos e tropas (simples)
    int donos[NUM_TERRITORIOS] = {
        COR_JOGADOR, COR_VERMELHA, COR_VERMELHA, COR_VERDE,
        COR_VERDE, COR_NEUTRA, COR_VERMELHA, COR_JOGADOR
    };
    int tropasIni[NUM_TERRITORIOS] = {5,4,3,3,2,1,4,6};

    for (int i = 0; i < quantidade && i < NUM_TERRITORIOS; ++i) {
        strncpy(mapa[i].nome, nomesPadrao[i], MAX_NOME-1);
        mapa[i].nome[MAX_NOME-1] = '\0';
        mapa[i].corDoExercito = donos[i];
        mapa[i].tropas = tropasIni[i];
    }
}

// Libera memoria
void liberarMemoria(Territorio* mapa) {
    if (mapa != nullptr) free(mapa);
}

// Exibe o mapa (somente leitura -> const)
void exibirMapa(const Territorio* mapa, int quantidade) {
    if (mapa == nullptr) return;
    std::cout << "Idx | Nome               | Dono  | Tropas\n";
    std::cout << "----+--------------------+-------+--------\n";
    for (int i = 0; i < quantidade; ++i) {
        const char* donoStr;
        switch (mapa[i].corDoExercito) {
            case COR_JOGADOR: donoStr = "Voce"; break;
            case COR_VERMELHA: donoStr = "Vermelha"; break;
            case COR_VERDE: donoStr = "Verde"; break;
            default: donoStr = "Neutro";
        }
        std::cout << (i<10?" ":"") << i << "  | "
                  << mapa[i].nome << (strlen(mapa[i].nome) < 18 ? std::string(18 - strlen(mapa[i].nome), ' ') : "")
                  << " | " << donoStr << (strlen(donoStr) < 6 ? std::string(6 - strlen(donoStr), ' ') : "")
                  << " | " << mapa[i].tropas << "\n";
    }
}

// Exibe menu principal
void exibirMenuPrincipal() {
    std::cout << "\n--- MENU PRINCIPAL ---\n";
    std::cout << "1 - Atacar (iniciar fase de ataque)\n";
    std::cout << "2 - Verificar missao / condicao de vitoria\n";
    std::cout << "0 - Sair\n";
}

// Exibe descricao da missao com base no ID
void exibirMissao(int missaoId, int corJogador) {
    if (missaoId == 0) {
        std::cout << "Missao: DESTRUIR um exército inimigo por completo (remover todas as tropas de uma cor inimiga).\n";
    } else if (missaoId == 1) {
        std::cout << "Missao: CONQUISTAR 4 territorios no mapa (ser o dono de pelo menos 4 territorios).\n";
    } else {
        std::cout << "Missao: Desconhecida.\n";
    }
}

// Gerencia a interface para escolher origem/destino e chama simularAtaque
void faseDeAtaque(Territorio* mapa, int quantidade, int corJogador) {
    if (mapa == nullptr) return;

    std::cout << "\n-- FASE DE ATAQUE --\n";
    std::cout << "Digite o indice do território de origem (onde voce tem tropas): ";
    int idxOrigem = lerInteiroSeguro();
    if (idxOrigem < 0 || idxOrigem >= quantidade) {
        std::cout << "Origem invalida.\n";
        return;
    }
    if (mapa[idxOrigem].corDoExercito != corJogador) {
        std::cout << "Voce nao controla o territorio de origem.\n";
        return;
    }
    if (mapa[idxOrigem].tropas < 2) {
        std::cout << "Precisa ter pelo menos 2 tropas para atacar (1 deve permanecer).\n";
        return;
    }

    std::cout << "Digite o indice do territorio de destino (inimigo ou neutro): ";
    int idxDestino = lerInteiroSeguro();
    if (idxDestino < 0 || idxDestino >= quantidade) {
        std::cout << "Destino invalido.\n";
        return;
    }
    if (idxDestino == idxOrigem) {
        std::cout << "Origem e destino iguais.\n";
        return;
    }

    // Chama a simulação passando ponteiros para origem e destino
    simularAtaque(&mapa[idxOrigem], &mapa[idxDestino], corJogador);
}

// Simula uma batalha simples e atualiza tropas/dono (usa ponteiros)
void simularAtaque(Territorio* origem, Territorio* destino, int corJogador) {
    if (origem == nullptr || destino == nullptr) return;

    std::cout << "\n--- Simulacao de Ataque ---\n";
    std::cout << "Origem: " << origem->nome << " (" << origem->tropas << " tropas)\n";
    std::cout << "Destino: " << destino->nome << " (" << destino->tropas << " tropas, dono: " << destino->corDoExercito << ")\n";

    // Simples rolagem de dados: atacante rola N dados = tropas-1, defensor rola M dados = min(2, tropasDef)
    int atacanteDados = origem->tropas - 1;
    if (atacanteDados > 3) atacanteDados = 3; // limite por rodada
    int defensorDados = destino->tropas >=2 ? 2 : destino->tropas;
    if (defensorDados < 1) defensorDados = 1;

    // Soma das rolagens
    int somaAtq = 0, somaDef = 0;
    for (int i = 0; i < atacanteDados; ++i) somaAtq += (rand() % 6) + 1;
    for (int i = 0; i < defensorDados; ++i) somaDef += (rand() % 6) + 1;

    std::cout << "Atacante (" << atacanteDados << " dados) soma: " << somaAtq << "\n";
    std::cout << "Defensor  (" << defensorDados << " dados) soma: " << somaDef << "\n";

    if (somaAtq > somaDef) {
        // atacante vence: remove 1-2 tropas do defensor
        int perdas = 1 + (rand() % 2);
        if (perdas > destino->tropas) perdas = destino->tropas;
        destino->tropas -= perdas;
        std::cout << "Atacante venceu! Defensor perde " << perdas << " tropas.\n";
    } else {
        // defensor vence: atacante perde 1 tropa
        origem->tropas -= 1;
        std::cout << "Defensor resistiu! Atacante perde 1 tropa.\n";
    }

    // Se destino ficar com 0 tropas, conquista
    if (destino->tropas <= 0) {
        std::cout << "Territorio " << destino->nome << " conquistado!\n";
        destino->corDoExercito = corJogador;
        // mover uma tropa do atacante para ocupar (deixa pelo menos 1 na origem)
        if (origem->tropas > 1) {
            origem->tropas -= 1;
            destino->tropas = 1;
            std::cout << "1 tropa movida de " << origem->nome << " para " << destino->nome << ".\n";
        } else {
            // se por alguma razao nao houver tropas sobrando, ocupa sem mover (caso extremo)
            destino->tropas = 0;
        }
    }
}

// Sorteia ID de missao aleatoria
int sortearMissao() {
    // 0 => destruir exército, 1 => conquistar X territorios
    return rand() % NUM_MISSOES;
}

// Verifica condicao de vitoria baseado na missao
bool verificarVitoria(const Territorio* mapa, int quantidade, int missaoId, int corJogador) {
    if (mapa == nullptr) return false;

    if (missaoId == 0) {
        // Missao: destruir um exército inimigo (qualquer cor diferente da do jogador)
        // Condicao: existir ao menos uma cor inimiga cujo total de tropas seja 0 ou nao exista território com essa cor.
        // Implementacao simples: pra cada cor inimiga conhecida, verifica se ha tropas dessa cor
        int somaVermelho = 0;
        int somaVerde = 0;
        for (int i = 0; i < quantidade; ++i) {
            if (mapa[i].corDoExercito == COR_VERMELHA) somaVermelho += mapa[i].tropas;
            if (mapa[i].corDoExercito == COR_VERDE) somaVerde += mapa[i].tropas;
        }
        // Se alguma das cores inimigas tiver soma == 0, consideramos missão cumprida
        if (somaVermelho == 0 || somaVerde == 0) return true;
        return false;
    } else if (missaoId == 1) {
        // Missao: conquistar N territorios (ex: 4)
        const int alvo = 4;
        int cont = 0;
        for (int i = 0; i < quantidade; ++i) {
            if (mapa[i].corDoExercito == corJogador) ++cont;
        }
        return (cont >= alvo);
    }

    return false;
}

// Limpa buffer de entrada (util para misturar cin/cstdio)
void limparBufferEntrada() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Le inteiro com tratamento basico para evitar loop infinito em caso de entrada invalida
int lerInteiroSeguro() {
    int valor;
    while (!(std::cin >> valor)) {
        std::cout << "Entrada invalida. Digite um numero inteiro: ";
        limparBufferEntrada();
    }
    limparBufferEntrada();
    return valor;
}