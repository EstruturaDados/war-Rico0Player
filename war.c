// ============================================================================
//      PROJETO WAR - RICO PLAYER 
// ============================================================================
//       AUTOR: Rico Player
//       DATA: 21/11/2025
//       DESCRIÇÃO: Versão Mestre com Interface Visual
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- DEFINIÇÃO DE CORES (ANSI CODES) ---
#define RESET   "\033[0m"
#define VERMELHO "\033[1;31m"
#define VERDE   "\033[1;32m"
#define AZUL    "\033[1;34m"
#define AMARELO "\033[1;33m"
#define CIANO   "\033[1;36m"
#define NEGRITO "\033[1m"

// --- Constantes Globais ---
#define QTD_TERRITORIOS 5
#define MAX_STR 30

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_STR];
    char cor[MAX_STR]; 
    int tropas;
} Territorio;

// --- Protótipos ---
Territorio* alocarMapa();
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa);
void exibirMissao(int idMissao);
void limparBufferEntrada();
void faseDeAtaque(Territorio *mapa);
void simularAtaque(Territorio *atacante, Territorio *defensor);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, int idMissao);
void limparTela();
void printComCor(char *texto, char *corDona);
void exibirTitulo();

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio *mapa = alocarMapa();
    if (mapa == NULL) return 1;

    inicializarTerritorios(mapa);
    int idMissao = sortearMissao();
    int opcao = 0;

    do {
        limparTela();
        exibirTitulo();
        
        printf(AMARELO ">>> JOGADOR: RICO PLAYER <<<\n" RESET);
        exibirMissao(idMissao);
        printf("\n");
        
        exibirMapa(mapa);
        exibirMenuPrincipal();
        
        printf(NEGRITO "Escolha sua estratégia: " RESET);
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 2:
                if (verificarVitoria(mapa, idMissao)) {
                    limparTela();
                    exibirTitulo();
                    printf("\n" AMARELO "******************************************\n");
                    printf("* PARABÉNS! MISSÃO CUMPRIDA!         *\n");
                    printf("* VITÓRIA DO EXÉRCITO AZUL! 🏆       *\n");
                    printf("******************************************\n" RESET);
                    opcao = 0; 
                } else {
                    printf("\n" VERMELHO "[!] A missão ainda não foi cumprida. O general exige mais empenho!\n" RESET);
                }
                break;
            case 0:
                printf("\nEncerrando o sistema de guerra...\n");
                break;
            default:
                printf("\n" VERMELHO "Comando inválido!\n" RESET);
        }

        if (opcao != 0) {
            printf("\n" CIANO "Pressione ENTER para continuar..." RESET);
            getchar();
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}

// --- Funções Visuais e Lógicas ---

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void exibirTitulo() {
    printf(CIANO);
    printf("================================================\n");
    // Desenho corrigido para um W mais claro
    printf(" __          __   _       _____   \n");
    printf(" \\ \\        / /  / \\     |  __ \\  \n");
    printf("  \\ \\  /\\  / /  / _ \\    | |__) | \n");
    printf("   \\ \\/  \\/ /  / ___ \\   |  _  /  \n");
    printf("    \\  /\\  /  /_/   \\_\\  | | \\ \\  \n");
    printf("     \\/  \\/              |_|  \\_\\ \n");
    printf("================================================\n");
    printf(RESET);
}

void printComCor(char *texto, char *corDona) {
    if (strcmp(corDona, "Azul") == 0) printf(AZUL "%s" RESET, texto);
    else if (strcmp(corDona, "Verde") == 0) printf(VERDE "%s" RESET, texto);
    else if (strcmp(corDona, "Vermelho") == 0) printf(VERMELHO "%s" RESET, texto);
    else printf("%s", texto);
}

Territorio* alocarMapa() {
    Territorio *ptr = (Territorio*) calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (ptr == NULL) printf(VERMELHO "Erro Crítico de Memória!\n" RESET);
    return ptr;
}

void inicializarTerritorios(Territorio *mapa) {
    strcpy(mapa[0].nome, "Brasil");    strcpy(mapa[0].cor, "Azul");     mapa[0].tropas = 5;
    strcpy(mapa[1].nome, "Argentina"); strcpy(mapa[1].cor, "Verde");    mapa[1].tropas = 3;
    strcpy(mapa[2].nome, "Argelia");   strcpy(mapa[2].cor, "Verde");    mapa[2].tropas = 2;
    strcpy(mapa[3].nome, "Espanha");   strcpy(mapa[3].cor, "Azul");     mapa[3].tropas = 4;
    strcpy(mapa[4].nome, "Japão");     strcpy(mapa[4].cor, "Vermelho"); mapa[4].tropas = 3;
}

void liberarMemoria(Territorio *mapa) {
    if (mapa != NULL) free(mapa);
}

void exibirMenuPrincipal() {
    printf("\n--- COMANDO CENTRAL ---\n");
    printf("[ 1 ] " VERMELHO "⚔️  ATACAR TERRITÓRIO\n" RESET);
    printf("[ 2 ] " AMARELO "🏆 VERIFICAR MISSÃO\n" RESET);
    printf("[ 0 ] " CIANO "🚪 SAIR\n" RESET);
}

void exibirMapa(const Territorio *mapa) {
    printf("\n--- 🗺️  MAPA TÁTICO GLOBAL ---\n");
    printf(" ID | TERRITÓRIO      | EXÉRCITO   | TROPAS \n");
    printf("--------------------------------------------\n");
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf(" %-2d | %-15s | ", i+1, mapa[i].nome);
        
        printComCor(mapa[i].cor, mapa[i].cor);
        
        int len = strlen(mapa[i].cor);
        for(int k=0; k < (11-len); k++) printf(" ");

        printf("| ");
        
        if (strcmp(mapa[i].cor, "Azul") == 0) printf(AZUL NEGRITO "%d ♟️" RESET, mapa[i].tropas);
        else printf("%d ♟️", mapa[i].tropas);
        
        printf("\n");
    }
    printf("--------------------------------------------\n");
}

void exibirMissao(int idMissao) {
    printf(NEGRITO "📜 MISSÃO SECRETA: " RESET);
    if (idMissao == 1) printf("Destruir o exército " VERDE "VERDE" RESET ".\n");
    else printf("Conquistar e manter " AMARELO "3 TERRITÓRIOS" RESET ".\n");
}

void faseDeAtaque(Territorio *mapa) {
    int idAtk, idDef;
    
    printf("\n" VERMELHO "--- ⚔️  SALA DE GUERRA ⚔️  ---\n" RESET);
    printf("Seus territórios (" AZUL "AZUL" RESET "):\n");
    for(int i=0; i<QTD_TERRITORIOS; i++) {
        if(strcmp(mapa[i].cor, "Azul") == 0) 
            printf("ID %d - %s (%d tropas)\n", i+1, mapa[i].nome, mapa[i].tropas);
    }

    printf("\nDigite o ID do seu atacante: ");
    scanf("%d", &idAtk);
    
    printf("Digite o ID do alvo inimigo: ");
    scanf("%d", &idDef);
    limparBufferEntrada();

    idAtk--; idDef--; 

    if (idAtk < 0 || idAtk >= QTD_TERRITORIOS || idDef < 0 || idDef >= QTD_TERRITORIOS) {
        printf(VERMELHO "Coordenadas inválidas!\n" RESET); return;
    }
    if (strcmp(mapa[idAtk].cor, "Azul") != 0) {
        printf(VERMELHO "Este exército não obedece a você!\n" RESET); return;
    }
    if (strcmp(mapa[idAtk].cor, mapa[idDef].cor) == 0) {
        printf(AMARELO "Fogo amigo não é permitido!\n" RESET); return;
    }

    simularAtaque(&mapa[idAtk], &mapa[idDef]);
}

void simularAtaque(Territorio *atacante, Territorio *defensor) {
    printf("\nIniciando combate...\n");
    printf(AZUL "%s" RESET " vs " VERMELHO "%s" RESET "\n", atacante->nome, defensor->nome);
    
    printf("Pressione ENTER para rolar os dados...");
    getchar();

    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("\n🎲 Dados de Ataque: [" AZUL " %d " RESET "]\n", dadoAtk);
    printf("🎲 Dados de Defesa: [" VERMELHO " %d " RESET "]\n", dadoDef);

    if (dadoAtk >= dadoDef) {
        printf("\n" VERDE ">>> VITORIA DO ATAQUE! <<<" RESET "\n");
        printf("Defensor perde 1 tropa.\n");
        defensor->tropas--;
        
        if (defensor->tropas <= 0) {
            printf(AMARELO "\n🏰 TERRITÓRIO CONQUISTADO! 🏰\n" RESET);
            printf("%s agora pertence ao Império AZUL!\n", defensor->nome);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1; 
        }
    } else {
        printf("\n" VERMELHO ">>> DEFESA VENCEU! <<<" RESET "\n");
        printf("Ataque repelido.\n");
    }
}

int sortearMissao() { return (rand() % 2) + 1; }

int verificarVitoria(const Territorio *mapa, int idMissao) {
    int i;
    if (idMissao == 1) {
        for (i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) return 0;
        }
        return 1;
    } else {
        int contagem = 0;
        for (i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) contagem++;
        }
        return (contagem >= 3) ? 1 : 0;
    }
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
