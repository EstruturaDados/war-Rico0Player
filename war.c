// ============================================================================
//      PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//       AUTOR: Rico Player
//       NÍVEL: Mestre
//       DATA: 21/11/2025
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define QTD_TERRITORIOS 5
#define MAX_STR 30

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_STR];
    char cor[MAX_STR]; // Ex: "Azul" (Jogador), "Verde" (Inimigo)
    int tropas;
} Territorio;

// --- Protótipos das Funções ---

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa);
void exibirMissao(int idMissao);
void limparBufferEntrada();

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio *mapa);
void simularAtaque(Territorio *atacante, Territorio *defensor);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, int idMissao);

// --- Função Principal (main) ---
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio *mapa = alocarMapa();
    if (mapa == NULL) return 1; // Encerra se falhar alocação

    inicializarTerritorios(mapa);
    
    // O Jogador será o "Azul"
    int idMissao = sortearMissao();
    int opcao = 0;

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        // Limpa a tela (opcional, funciona em alguns sistemas)
        // system("cls || clear"); 
        
        printf("\n=== WAR: Nível Mestre (Rico Player) ===\n");
        exibirMissao(idMissao);
        exibirMapa(mapa);
        exibirMenuPrincipal();
        
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 2:
                if (verificarVitoria(mapa, idMissao)) {
                    printf("\n>>> PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO! <<<\n");
                    printf(">>> VITÓRIA DO EXÉRCITO AZUL! <<<\n");
                    opcao = 0; // Encerra o jogo
                } else {
                    printf("\n[!] A missão ainda não foi cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    // 3. Limpeza:
    liberarMemoria(mapa);
    printf("Memória liberada com sucesso.\n");

    return 0;
}

// --- Implementação das Funções ---

// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
Territorio* alocarMapa() {
    Territorio *ptr = (Territorio*) calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (ptr == NULL) {
        printf("Erro Crítico: Falha na alocação de memória!\n");
    }
    return ptr;
}

// Preenche os dados iniciais. Vamos definir o Jogador como AZUL.
void inicializarTerritorios(Territorio *mapa) {
    // Território 1 (Do Jogador)
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, "Azul");
    mapa[0].tropas = 5;

    // Território 2 (Inimigo Verde)
    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].cor, "Verde");
    mapa[1].tropas = 3;

    // Território 3 (Inimigo Verde)
    strcpy(mapa[2].nome, "Argelia");
    strcpy(mapa[2].cor, "Verde");
    mapa[2].tropas = 2;

    // Território 4 (Do Jogador)
    strcpy(mapa[3].nome, "Espanha");
    strcpy(mapa[3].cor, "Azul");
    mapa[3].tropas = 4;

    // Território 5 (Inimigo Vermelho/Neutro)
    strcpy(mapa[4].nome, "Japão");
    strcpy(mapa[4].cor, "Vermelho");
    mapa[4].tropas = 3;
}

// Libera a memória previamente alocada.
void liberarMemoria(Territorio *mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

// Imprime na tela o menu de ações.
void exibirMenuPrincipal() {
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1. Atacar Território\n");
    printf("2. Verificar Missão (Vitória)\n");
    printf("0. Sair do Jogo\n");
}

// Mostra o estado atual. Usa 'const' para garantir leitura apenas.
void exibirMapa(const Territorio *mapa) {
    printf("\n--- MAPA MUNDI ---\n");
    printf("%-4s | %-15s | %-10s | %s\n", "ID", "Território", "Dono", "Tropas");
    printf("------------------------------------------------\n");
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("%-4d | %-15s | %-10s | %d\n", 
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------------------------------------\n");
}

// Exibe a descrição da missão atual.
void exibirMissao(int idMissao) {
    printf("\n>>> MISSÃO SECRETA: ");
    if (idMissao == 1) {
        printf("Destruir totalmente o exército VERDE.\n");
    } else {
        printf("Conquistar e manter 3 territórios.\n");
    }
}

// Gerencia a interface para a ação de ataque.
void faseDeAtaque(Territorio *mapa) {
    int idAtk, idDef;
    
    printf("\n--- FASE DE COMANDO DE ATAQUE ---\n");
    printf("Digite o ID do seu território atacante (Azul): ");
    scanf("%d", &idAtk);
    
    printf("Digite o ID do território alvo: ");
    scanf("%d", &idDef);
    limparBufferEntrada();

    // Ajuste de índice (1-5 para 0-4)
    idAtk--; 
    idDef--;

    // Validações básicas
    if (idAtk < 0 || idAtk >= QTD_TERRITORIOS || idDef < 0 || idDef >= QTD_TERRITORIOS) {
        printf("Erro: Territórios inexistentes.\n");
        return;
    }
    
    // Regra: Só pode atacar com seu próprio exército (Azul)
    if (strcmp(mapa[idAtk].cor, "Azul") != 0) {
        printf("Erro: Você só pode comandar o exército Azul!\n");
        return;
    }

    // Regra: Não pode atacar a si mesmo
    if (strcmp(mapa[idAtk].cor, mapa[idDef].cor) == 0) {
        printf("Erro: Não é possível atacar um território aliado.\n");
        return;
    }

    simularAtaque(&mapa[idAtk], &mapa[idDef]);
}

// Executa a lógica de uma batalha entre dois territórios.
void simularAtaque(Territorio *atacante, Territorio *defensor) {
    printf("\nIniciando batalha: %s (Azul) vs %s (%s)...\n", 
           atacante->nome, defensor->nome, defensor->cor);

    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Dados: Ataque [%d] x Defesa [%d]\n", dadoAtk, dadoDef);

    // Regra MateCheck: Empate favorece atacante
    if (dadoAtk >= dadoDef) {
        printf("Resultado: VITORIA DO ATAQUE!\n");
        defensor->tropas--;
        
        // Verifica Conquista
        if (defensor->tropas <= 0) {
            printf(">>> TERRITÓRIO CONQUISTADO! <<<\n");
            printf("O exército Azul dominou %s!\n", defensor->nome);
            
            // Troca a cor e define tropa mínima
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1; 
        }
    } else {
        printf("Resultado: DEFESA VENCEU! Ataque recua.\n");
    }
}

// Sorteia ID de missão aleatório (1 ou 2).
int sortearMissao() {
    // Retorna 1 ou 2
    return (rand() % 2) + 1;
}

// Verifica se o jogador cumpriu os requisitos.
int verificarVitoria(const Territorio *mapa, int idMissao) {
    int i;
    
    if (idMissao == 1) {
        // MISSÃO 1: Destruir o exército Verde
        // Lógica: Percorrer o mapa. Se achar ALGUM "Verde", não venceu ainda.
        for (i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) {
                return 0; // Ainda existe verde
            }
        }
        return 1; // Não achou nenhum verde, Vitória!
    } 
    else {
        // MISSÃO 2: Conquistar 3 territórios
        // Lógica: Contar quantos "Azul" existem.
        int contagem = 0;
        for (i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) {
                contagem++;
            }
        }
        
        if (contagem >= 3) return 1;
        else return 0;
    }
}

// Limpa buffer do teclado
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
