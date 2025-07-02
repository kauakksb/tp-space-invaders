#include "libs_e_tipos.h"

// Armazena a matriz do desenho de uma figura
int ** guarda_matriz(int *linhas, int *colunas, char * nome_arquivo);

// Verifica se o tiro da nave colidiu com algum alien
int colisao_tiro(TIRO * municao, float novo_x, float novo_y, GRUPO_ALIENS ** aliens, int tamanho_esq, int n);

// Verifica se algum alien está muito próximo da nave
int alien_proximo_nave(NAVE * nave, GRUPO_ALIENS ** aliens, int tamanho_esq, int num_esq);

// Verifica se todos os aliens foram atingidos
int rodada_acabou(GRUPO_ALIENS **aliens, int n);

// Verifica se o score foi superior ao recorde e caso seja armazena num arquivo
int guarda_recorde(int score, int recorde);