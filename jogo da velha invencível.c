#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define jogador 'X'
#define bot 'O'
#define vazio ' '

char tabuleiro[3][3]; // tabuleiro
char nomeDeJogador[50]; // nome do jogador

// mostra o tabuleiro
void mostrarTabuleiro() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", tabuleiro[i][j]);
            if (j < 2) printf("|");
        }
        if (i < 2) printf("\n---|---|---\n");
    }
    printf("\n");
}

// inicia o tabuleiro
void iniciaTabuleiro() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            tabuleiro[i][j] = vazio;
}

// Verifica se é possível vencer naquela linha
int linhaInvencivel(int lin1, int col1, int lin2, int col2, int lin3, int col3) {
    char simbolo[3] = { tabuleiro[lin1][col1], tabuleiro[lin2][col2], tabuleiro[lin3][col3] };

    // Se houver símbolos diferentes na mesma linha, então não é possível vencer
    if ((simbolo[0] == jogador && simbolo[1] == bot) || 
        (simbolo[0] == bot && simbolo[1] == jogador) || 
        (simbolo[1] == jogador && simbolo[2] == bot) || 
        (simbolo[1] == bot && simbolo[2] == jogador) || 
        (simbolo[0] == jogador && simbolo[2] == bot) || 
        (simbolo[0] == bot && simbolo[2] == jogador)) {
        return 1; // Linha invencível
    }
    return 0;
}

// Verifica se todas as linhas são invencíveis ou se o tabuleiro está cheio
int checaVelhou() {
    // Verifica linhas
    for (int i = 0; i < 3; i++) {
        if (!linhaInvencivel(i, 0, i, 1, i, 2)) return 0; // Linha ainda pode ter vitória
        if (!linhaInvencivel(0, i, 1, i, 2, i)) return 0; // Coluna ainda pode ter vitória
    }
    // Verifica diagonais
    if (!linhaInvencivel(0, 0, 1, 1, 2, 2)) return 0; // Diagonal principal
    if (!linhaInvencivel(0, 2, 1, 1, 2, 0)) return 0; // Diagonal secundária

    return 1; // Todas as linhas são invencíveis, logo o jogo empatou
}

// Verifica se alguém ganhou
int checaVitoria(char simbolo) {
    for (int i = 0; i < 3; i++) {
        // Verifica as linhas e colunas
        if ((tabuleiro[i][0] == simbolo && tabuleiro[i][1] == simbolo && tabuleiro[i][2] == simbolo) ||
            (tabuleiro[0][i] == simbolo && tabuleiro[1][i] == simbolo && tabuleiro[2][i] == simbolo))
            return 1;
    }
    // Verifica as diagonais
    if ((tabuleiro[0][0] == simbolo && tabuleiro[1][1] == simbolo && tabuleiro[2][2] == simbolo) ||
        (tabuleiro[0][2] == simbolo && tabuleiro[1][1] == simbolo && tabuleiro[2][0] == simbolo))
        return 1;
    return 0;
}

// Verifica se o tabuleiro está cheio
int tabuleiroCheio() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (tabuleiro[i][j] == vazio)
                return 0;
    return 1;
}

// Verifica se o jogo acabou (ganhou ou velhou)
int jogoAcabou() {
    return checaVitoria(jogador) || checaVitoria(bot) || checaVelhou();
}

// Função com as prioridades para o bot jogar
void movimentoBot() {
    // Prioridade 1: Jogar no centro se estiver vazio
    if (tabuleiro[1][1] == vazio) {
        tabuleiro[1][1] = bot;
        return;
    }

    // Prioridade 2.1: Verifica se pode ganhar
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == vazio) {
                tabuleiro[i][j] = bot;
                if (checaVitoria(bot)) return; // Venceu
                tabuleiro[i][j] = vazio; // Reverte se não venceu
            }
        }
    }

    // Prioridade 2.2: Verifica se o jogador pode vencer e bloqueia
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i][j] == vazio) {
                tabuleiro[i][j] = jogador;
                if (checaVitoria(jogador)) {
                    tabuleiro[i][j] = bot; // Bloqueia o jogador
                    return;
                }
                tabuleiro[i][j] = vazio; // Reverte se não precisa bloquear
            }
        }
    }

    // Prioridade 3: Verifica se o jogador marcou dois cantos opostos
    int cantos[4][2] = { {0, 0}, {0, 2}, {2, 0}, {2, 2} };
    int oposto[4][2] = { {2, 2}, {2, 0}, {0, 2}, {0, 0} };
    for (int i = 0; i < 4; i++) {
        int lin = cantos[i][0], col = cantos[i][1];
        int linOposto = oposto[i][0], colOposto = oposto[i][1];
        if (tabuleiro[lin][col] == jogador && tabuleiro[linOposto][colOposto] == jogador) {
            // Tenta ocupar uma extremidade da cruz
            if (tabuleiro[1][0] == vazio) {
                tabuleiro[1][0] = bot; // Centro da coluna
                return;
            }
            if (tabuleiro[0][1] == vazio) {
                tabuleiro[0][1] = bot; // Centro da linha
                return;
            }
            if (tabuleiro[2][1] == vazio) {
                tabuleiro[2][1] = bot; // Centro da linha
                return;
            }
            if (tabuleiro[1][2] == vazio) {
                tabuleiro[1][2] = bot; // Centro da coluna
                return;
            }
        }
    }

    // Prioridade 4: Se o centro estiver ocupado, joga em uma das pontas
    int canto[4][2] = { {0, 0}, {0, 2}, {2, 0}, {2, 2} };
    for (int i = 0; i < 4; i++) {
        int lin = canto[i][0], col = canto[i][1];
        if (tabuleiro[lin][col] == vazio) {
            tabuleiro[lin][col] = bot;
            return;
        }
    }

    // Prioridade 5: Se nenhuma das opções anteriores for possível, joga no primeiro espaço vazio
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (tabuleiro[i][j] == vazio) {
                tabuleiro[i][j] = bot;
                return;
            }
}

// chamando a função jogar para não entrar em conflito com com a função de reiniciar
void jogar();

// Função para realizar a jogada do jogador ou reiniciar o jogo
void movimentoJogador() {
    char escolha;
    do {
        printf("Jogador ou reiniciar (j/r): ");
        scanf(" %c", &escolha);

        // Se o jogador escolher reiniciar
        if (escolha == 'r' || escolha == 'R') {
            printf("Reiniciando o jogo...\n");
            iniciaTabuleiro();   // Reinicia o tabuleiro
            mostrarTabuleiro();  // Mostra o tabuleiro reiniciado
            jogar();             // Inicia uma nova partida
            return;              // Retorna para sair da função atual e começar o novo jogo
        }

        // Se o jogador optar por jogar
        if (escolha == 'j' || escolha == 'J') {
            int lin, col;
            do {
                printf("Sua vez (%s):\n", nomeDeJogador);
                printf("Escolha a linha (1-3) e coluna (1-3): ");
                scanf("%d%d", &lin, &col);
                lin--; col--;
            } while (lin < 0 || lin > 2 || col < 0 || col > 2 || 
                     tabuleiro[lin][col] != vazio);
            tabuleiro[lin][col] = jogador;
            break;  // Sai do loop ao realizar uma jogada válida
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (1);  // Continua perguntando até uma escolha válida ser feita
}

// Função para perguntar se o jogador deseja jogar novamente
int reiniciarJogo() {
    char resposta;
    printf("Deseja jogar novamente? (s/n): ");
    scanf(" %c", &resposta);
    return (resposta == 's' || resposta == 'S');
}

// fluxo de jogo
void jogar() {
    int primeiro = rand() % 2;
    printf("%s começa jogando!\n", primeiro ? nomeDeJogador : "Computador");

    while (!jogoAcabou()) {
        if (primeiro) {
            movimentoJogador();
            mostrarTabuleiro();
            if (checaVitoria(jogador)) {
                printf("Parabéns %s, você venceu!\n", nomeDeJogador);
                break;
            }
            primeiro = 0; // vez máquina
        } else {
            movimentoBot();
            mostrarTabuleiro();
            if (checaVitoria(bot)) {
                printf("O computador venceu!\n");
                break;
            }
            primeiro = 1; // vez jogador
        }

        if (checaVelhou()) {
            printf("Velhou! Não há mais possibilidade de vitória.\n");
            break;
        }
    }

    // Pergunta se deseja reiniciar
    if (reiniciarJogo()) {
        iniciaTabuleiro();
        mostrarTabuleiro();
        jogar(); // Reinicia o jogo
    } else {
        printf("Voltando ao menu...\n");
    }
}

// Função do menu
void menu() {
    int escolha;
    do {
        printf("\nJogo da velha invencível:\n");
        printf("1. Iniciar nova partida\n");
        printf("2. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Digite o nome do jogador: ");
                scanf("%s", nomeDeJogador);
                iniciaTabuleiro();
                mostrarTabuleiro();
                jogar();
                break;
            case 2:
                printf("Saindo...\n");
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    } while (escolha != 2);
}

int main() {
    srand(time(0)); // Para garantir aleatoriedade
    menu();
    return 0;
}