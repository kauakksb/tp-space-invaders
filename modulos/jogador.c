#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "nave.h"

typedef struct JOGADOR
{
    char *nome;
    int id;
    int vidas;
    int score;
    NAVE *nave;
}JOGADOR;

JOGADOR * initJogador(char * nome,int id, int vidas)
{
    JOGADOR * jogador = (JOGADOR*) malloc(sizeof(JOGADOR));

    jogador->nome = (char*) malloc(strlen(nome) * sizeof(char));
    strcpy(jogador->nome, nome);

    jogador->id = id;
    jogador->vidas = vidas;
    jogador->score = 0;

    jogador->nave = initNave();
}

void destroi_jogador(JOGADOR * jogador)
{
    destroi_nave(jogador->nave);
    free(jogador->nome);
    free(jogador);
}