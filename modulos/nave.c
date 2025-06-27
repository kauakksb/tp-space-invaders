#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "util.h"

#define SCREEN_W 960 // Largura da tela
#define SCREEN_H 540 // Altura da tela
#define GRASS_H 60

#define NAVE_W 100 
#define NAVE_H 50

#define PONTA_NAVE SCREEN_H - (GRASS_H/2) - NAVE_H


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
NAVE * initNave(char * tipo)
{
    NAVE *nave = (NAVE*) malloc(sizeof(NAVE));

    if(!strcmp(tipo, "nv1"))
    {
        nave->desenho = guarda_desenho(&nave->linhas, &nave->colunas, "designs/nv1.txt");
    }
    else if(!strcmp(tipo, "nv2"))
    {
        nave->desenho = guarda_desenho(&nave->linhas, &nave->colunas, "designs/nv2.txt");
    }

    nave->altura = 3 * nave->linhas;
    nave->largura = 3 * nave->colunas;

    nave->x = (float)(SCREEN_W/2) - (nave->largura/2);
    nave->y = SCREEN_H - (GRASS_H/2) - nave->altura;
    nave->vel = 3;
    nave->dir = 0;
    nave->esq = 0;
    nave->cor = al_map_rgb(0, 0, 255);

    nave->municao.estado = 0;
    nave->municao.raio = 5;
    nave->municao.x = nave->x + (nave->largura/2);
    nave->municao.y = nave->y;
    nave->municao.y_vel = 4;
    nave->municao.cor = al_map_rgb(255, 0, 0);
    printf("Nave criada\n");


    return nave;
}

// Desenha a nave do jogador no cenário
void draw_nave(NAVE *nave)
{
    for(int i = 0; i < nave->linhas; i++)
    {
        for(int j = 0; j < nave->colunas; j++)
        {
            if(nave->desenho[i][j] == 1)
            {
                al_draw_filled_rectangle(nave->x + (j*3), nave->y + (i*3), 
                                nave->x + (j+1) * 3, nave->y + (i+1) * 3, 
                                nave->cor);
            }
            
        }
    }
    // al_draw_filled_triangle(nave->x, y_base - NAVE_H, 
    //                         nave->x - (NAVE_W/2), y_base, 
    //                         nave->x + (NAVE_W/2), y_base, 
    //                         nave->cor);
}

// Atualiza a posição da nave de acordo com parâmetros de controle da nave
void update_nave(NAVE *nave)
{

    printf("endereco de nave: %p", nave);

    printf("tentando atualizar nave\n");
    printf("nave esq: %d\n", nave->esq);
    printf("nave dir: %d\n", nave->dir);
    printf("nave x: %f", nave->x);
    if(nave->esq != nave->dir)
    {
        if(nave->esq == 1 && nave->x - nave->vel >= 0)
        {
            printf("Nave x: %f", nave->x);
            nave->x -= nave->vel;
        }
        else if(nave->dir == 1 && nave->x + nave->largura + nave->vel <= SCREEN_W)
        {
            printf("Nave x: %f", nave->x);
            nave->x += nave->vel;
        }

        if(nave->municao.estado == 0)
        {
            nave->municao.x = nave->x;
        }
    }
}

// Desenha o tiro do jogador no cenário
void draw_tiro(TIRO municao)
{
    if(municao.estado == 1)
    {
        al_draw_filled_circle(municao.x, municao.y, municao.raio, municao.cor);
    }
}

// Atualiza a posição do tiro e retorna para nave caso não acerte nenhum alvo
void update_tiro(TIRO * municao, float novo_x)
{
    if(municao->y + municao->y_vel <= 0)
    {
        municao->estado = 0;
        municao->x = novo_x;
        municao->y = PONTA_NAVE;
    }

    if(municao->estado == 1)
    {
        municao->y -= municao->y_vel;
    }
    
}

// Libera a memória alocada para a struct nave e o seu design
void destroi_nave(NAVE * nave)
{
    for(int i = 0; i < nave->linhas; i++)
    {
        free(nave->desenho[i]);
    }
    free(nave->desenho);
    free(nave);
}