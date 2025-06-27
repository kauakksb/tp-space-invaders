#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>

/* 
    Struct que compõe a munição do jogador, com propriedades para controlar 
    sua posição e movimentação, bem como se a munição está sendo usada ou não
*/
typedef struct TIRO
{
    int estado;
    float x, y;
    float y_vel;
    float raio;
    ALLEGRO_COLOR cor;
}TIRO;

/* 
    Struct que compõe a nave controlada pelo jogador 
    e as propriedades da nave como posição e visual
*/
typedef struct NAVE
{
    int **desenho;
    int linhas, colunas;
    float altura, largura;
    float x, y;
    float vel;
    int dir, esq;
    ALLEGRO_COLOR cor;
    TIRO municao;
}NAVE;

// Instancia uma struct nave
NAVE * initNave(char * tipo);

// Desenha a nave do jogador no cenário
void draw_nave(NAVE *nave);

// Atualiza a posição da nave de acordo com parâmetros de controle da nave
void update_nave(NAVE *nave);

// Desenha o tiro do jogador no cenário
void draw_tiro(TIRO municao);

// Atualiza a posição do tiro e retorna para nave caso não acerte nenhum alvo
void update_tiro(TIRO * municao, float novo_x);

// Libera a memória alocada para a struct nave e o seu design
void destroi_nave(NAVE * nave);