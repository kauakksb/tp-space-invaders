
#ifndef LIBS_E_TIPOS_H
#define LIBS_E_TIPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

/* 
    Struct que contém uma matriz de pontos que representam o design, 
    junto de outras propriedades importantes para controle destes designs
*/
typedef struct DESIGN
{
    int **desenho;
    float tamanho_px;
    int linhas, colunas;
    float x, y;
    char * tipo;
}DESIGN;

/* 
    Struct usada para representar um botão e alguns elementos importantes de sua estrutura
*/
typedef struct BUTTON
{
    int estado;
    float x, y;
    float altura, largura;
    float padding_x, padding_y;
    char * texto;
    ALLEGRO_FONT * fonte;
    ALLEGRO_COLOR cor;
}BUTTON;

/* 
    Struct que compõe a munição do jogador, com propriedades para controlar 
    sua posição e movimentação, bem como se a munição está sendo usada ou não
*/
typedef struct TIRO
{
    int estado;
    int **desenho;
    int linhas, colunas;
    float altura, largura;
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

/* 
    Struct que compoe o jogador e sua nave que será usada durante a jogatina
*/
typedef struct JOGADOR
{
    char *nome;
    int id;
    int vidas;
    int score;
    int contador_pw;
    int num_tiros_pw;
    NAVE *nave;
}JOGADOR;

/*  
    Struct que compõe um alien e suas propriedades, bem como 
    outros elementos para controle de cada um no jogo 
*/
typedef struct ALIEN
{
    char *tipo;
    int **desenho;
    int estado;
    int id, grupo;
    int linhas, colunas;
    float altura, largura;
    float x, y;
    float x_vel, y_vel;
    ALLEGRO_COLOR cor;
}ALIEN;

/*  
    Struct que contém um esquadrão/vetor de aliens e as propriedades desse 
    grupo, que são usadas para controlar aspectos como a movimentação do 
    grupo e os sistemas do jogo 
*/
typedef struct GRUPO_ALIENS
{
    ALIEN **esquadrao;
    char *tipo;
    int id_grupo;
    int estado;
    float x, y;
    float espacamento_x;
    float espacamento_y;
    float altura, largura;
    int num_aliens;
}GRUPO_ALIENS;

#endif