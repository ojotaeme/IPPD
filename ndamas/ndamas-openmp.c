#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

// Variáveis globais
static int nSolutions = 0;    // Total de soluções (saída)
static int TamTabuleiro;      // Tamanho do tabuleiro (entrada)

/*
 * Checa se é permitido colocar a dama em uma determinada posição dada por
 * (row, col)
 * 
 * Retorna falso (0) se não é permitido e 1 caso contrário
 */
int isSafe(int *board, int row, int col) {
    int i, j;

    // Checa a fileira (esquerda)
    for (i = 0; i < col; i++)
        if (board[i] == row)
            return 0;

    // Checa a diagonal superior esquerda
    for (i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
        if (board[j] == i)
            return 0;

    // Checa a diagonal inferior esquerda
    for (i = row + 1, j = col - 1; j >= 0 && i < TamTabuleiro; i++, j--)
        if (board[j] == i)
            return 0;

    return 1;
}

/*
 * Resolve o problema N-Queens para uma coluna específica
 */
void solveNQ(int *board, int col, int *localSolutions) {
    for (int i = 0; i < TamTabuleiro; i++) {
        // Podemos colocar a dama na i-ésima fileira da coluna?
        if (isSafe(board, i, col)) {
            // Sim, então colocamos
            board[col] = i;

            // Última coluna -> solução encontrada
            if (col == TamTabuleiro - 1) {
                (*localSolutions)++;
            } else {
                // Checa recursivamente a próxima coluna
                solveNQ(board, col + 1, localSolutions);
            }
        }
    }
}

/*
 * Programa principal - entrada: N (tamanho do tabuleiro)
 */
int main(int argc, char *argv[]) {
    struct timeval start, stop;

    if (argc <= 1) {
        fprintf(stdout, "É necessário especificar o tamanho do tabuleiro\n");
        exit(-1);
    }
    TamTabuleiro = strtol(argv[1], NULL, 10);

    gettimeofday(&start, NULL);

    // Paraleliza as primeiras chamadas de solveNQ
    #pragma omp parallel
    {
        int *board = (int *)malloc(TamTabuleiro * sizeof(int));
        int localSolutions = 0;

        #pragma omp for schedule(dynamic)
        for (int i = 0; i < TamTabuleiro; i++) {
            board[0] = i;
            solveNQ(board, 1, &localSolutions);
        }

        #pragma omp atomic
        nSolutions += localSolutions;

        free(board);
    }

    gettimeofday(&stop, NULL);

    fprintf(stdout, "Número total de soluções: %d\n", nSolutions);

    double t = (((double)(stop.tv_sec) * 1000.0 + (double)(stop.tv_usec / 1000.0)) -
                ((double)(start.tv_sec) * 1000.0 + (double)(start.tv_usec / 1000.0)));

    fprintf(stdout, "Tempo decorrido = %g ms\n", t);

    return 0;
}
