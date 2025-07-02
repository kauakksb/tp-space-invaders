#include "libs_e_tipos.h"
#include "util.h"

#define SCREEN_W 960 // Largura da tela
#define SCREEN_H 540 // Altura da tela
#define GRASS_H 60

#define NAVE_W 100 
#define NAVE_H 50

#define PONTA_NAVE SCREEN_H - (GRASS_H/2) - NAVE_H


// Instancia uma struct nave
NAVE * initNave(char * tipo)
{
    NAVE *nave = (NAVE*) malloc(sizeof(NAVE));

    if(!strcmp(tipo, "nv1"))
    {
        nave->desenho = guarda_matriz(&nave->linhas, &nave->colunas, "designs/nv1.txt");
    }
    else if(!strcmp(tipo, "nv2"))
    {
        nave->desenho = guarda_matriz(&nave->linhas, &nave->colunas, "designs/nv2.txt");
    }

    nave->altura = 3 * nave->linhas;
    nave->largura = 3 * nave->colunas;

    nave->x = (float)(SCREEN_W/2) - (nave->largura/2);
    nave->y = SCREEN_H - (GRASS_H/2) - nave->altura;
    nave->vel = 3;
    nave->dir = 0;
    nave->esq = 0;
    nave->cor = al_map_rgb(0, 0, 255);

    nave->municao.desenho = guarda_matriz(&nave->municao.linhas, &nave->municao.colunas, "designs/tiro.txt");

    nave->municao.altura = 2 * nave->municao.linhas;
    nave->municao.largura = 2 * nave->municao.colunas;

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
}

// Atualiza a posição da nave de acordo com parâmetros de controle da nave
void update_nave(NAVE *nave)
{
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

// Reseta as variáveis de nave para uma nova rodada
void reset_nave(NAVE * nave)
{
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
}

// Desenha o tiro do jogador no cenário
void draw_tiro(TIRO municao)
{
    if(municao.estado == 1)
    {
        for(int i = 0; i < municao.linhas; i++)
        {
            for(int j = 0; j < municao.colunas; j++)
            {
                if(municao.desenho[i][j] == 1)
                {
                    al_draw_filled_rectangle(municao.x + (j*2), municao.y + (i*2), 
                                municao.x + (j+1) * 2, municao.y + (i+1) * 2, 
                                al_map_rgb(244, 154, 105));
                }
                else if(municao.desenho[i][j] == 2)
                {
                    al_draw_filled_rectangle(municao.x + (j*2), municao.y + (i*2), 
                                municao.x + (j+1) * 2, municao.y + (i+1) * 2, 
                                al_map_rgb(248, 244, 11));
                }
                else if (municao.desenho[i][j] == 3)
                {
                    al_draw_filled_rectangle(municao.x + (j*2), municao.y + (i*2), 
                                municao.x + (j+1) * 2, municao.y + (i+1) * 2, 
                                al_map_rgb(241, 51, 109));
                }
                else if (municao.desenho[i][j] == 4)
                {
                    al_draw_filled_rectangle(municao.x + (j*2), municao.y + (i*2), 
                                municao.x + (j+1) * 2, municao.y + (i+1) * 2, 
                                al_map_rgb(66, 4, 104));
                }
                
            }
        }
        
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