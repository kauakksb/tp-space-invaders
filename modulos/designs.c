#include "libs_e_tipos.h"
#include "util.h"

DESIGN* init_design(float x, float y, char * tipo, float tamanho_px)
{
    DESIGN * design = (DESIGN*) malloc(sizeof(DESIGN));
    design->x = x;
    design->y = y;
    design->tamanho_px = tamanho_px;
    design->tipo = (char*) malloc(strlen(tipo) * sizeof(char));
    strcpy(design->tipo, tipo);

    if(!strcmp(tipo, "star1"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/star.txt");
    }
    else if(!strcmp(tipo, "star2"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/star2.txt");
    }
    else if(!strcmp(tipo, "asteroide"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/asteroide.txt");
    }
    else if(!strcmp(tipo, "asteroide2"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/asteroide2.txt");
    }
    else if(!strcmp(tipo, "terra"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/terra.txt");
    }
    else if(!strcmp(tipo, "marte"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/marte.txt");
    }
    else if(!strcmp(tipo, "polvo"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/octopus.txt");
    }
    else if(!strcmp(tipo, "carangueijo"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/crab.txt");
    }
    else if(!strcmp(tipo, "nave1"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/nv1.txt");
    }
    else if(!strcmp(tipo, "nave2"))
    {
        design->desenho = guarda_matriz(&design->linhas, &design->colunas, "designs/nv2.txt");
    }

    return design;
}

void draw_design(DESIGN * design)
{
    ALLEGRO_COLOR cor1;
    ALLEGRO_COLOR cor2;
    ALLEGRO_COLOR cor3;
    if(!strcmp(design->tipo, "star1"))
    {
        cor1 = al_map_rgb(255, 255, 255);
        cor2 = al_map_rgb(255, 253, 123);
        cor3 = al_map_rgb(199, 199, 199);
    }
    else if(!strcmp(design->tipo, "star2"))
    {
        cor1 = al_map_rgb(255, 255, 255);
        cor2 = al_map_rgb(255, 253, 123);
        cor3 = al_map_rgb(199, 199, 199);
    }
    else if(!strcmp(design->tipo, "asteroide"))
    {
        cor1 = al_map_rgb(29, 33, 37);
        cor2 = al_map_rgb(108, 126, 139);
        cor3 = al_map_rgb(52, 59, 64);
    }
    else if(!strcmp(design->tipo, "asteroide2"))
    {
        cor1 = al_map_rgb(29, 33, 37);
        cor2 = al_map_rgb(108, 126, 139);
        cor3 = al_map_rgb(52, 59, 64);
    }
    else if(!strcmp(design->tipo, "terra"))
    {
        cor1 = al_map_rgb(1, 44, 91);
        cor2 = al_map_rgb(6, 131, 186);
        cor3 = al_map_rgb(105, 189, 9);
    }
    else if(!strcmp(design->tipo, "marte"))
    {
        cor1 = al_map_rgb(195, 36, 54);
        cor2 = al_map_rgb(224, 58, 52);
        cor3 = al_map_rgb(238, 107, 57);
    }
    else if(!strcmp(design->tipo, "polvo"))
    {
        cor1 = al_map_rgb(42, 198, 229);
        cor2 = al_map_rgb(0, 0, 255);
    }
    else if(!strcmp(design->tipo, "carangueijo"))
    {
        cor1 = al_map_rgb(253, 113, 82);
    }
    else if(!strcmp(design->tipo, "nave1") || !strcmp(design->tipo, "nave2"))
    {
        cor1 = al_map_rgb(0, 0, 255);
    }
    
    for(int i = 0; i < design->linhas; i++)
    {
        for(int j = 0; j < design->colunas; j++)
        {
            if(design->desenho[i][j] == 1)
            {
                al_draw_filled_rectangle(design->x + (j * design->tamanho_px), design->y + (i * design->tamanho_px),
                                        design->x + (j+1) * design->tamanho_px , design->y +(i+1) * design->tamanho_px, 
                                        cor1);
            }
            else if(design->desenho[i][j] == 2)
            {
                al_draw_filled_rectangle(design->x + (j * design->tamanho_px), design->y + (i * design->tamanho_px),
                                        design->x + (j+1) * design->tamanho_px , design->y +(i+1) * design->tamanho_px,
                                        cor2);
            }
            else if (design->desenho[i][j] == 3)
            {
                al_draw_filled_rectangle(design->x + (j * design->tamanho_px), design->y + (i * design->tamanho_px),
                                        design->x + (j+1) * design->tamanho_px , design->y +(i+1) * design->tamanho_px,
                                        cor3);
            }
        }
    }
}

void destroi_design(DESIGN * design)
{
    for(int i = 0; i < design->linhas; i++)
    {
        free(design->desenho[i]);
    }
    free(design->desenho);
    free(design->tipo);
    free(design);
}

BUTTON * init_button(
    char * texto, int estado, float x, float y, float padding_x, float padding_y, 
    float altura,float largura, ALLEGRO_FONT * fonte, ALLEGRO_COLOR cor)
{
    BUTTON *botao = (BUTTON*) malloc(sizeof(BUTTON));

    botao->texto = (char*) malloc(strlen(texto) * sizeof(char));
    strcpy(botao->texto, texto);

    botao->estado = estado;

    botao->x = x;
    botao->y = y;

    botao->altura = altura;
    botao->largura = largura;

    botao->padding_x = padding_x;
    botao->padding_y = padding_y;

    botao->fonte = fonte;
    botao->cor = cor;

    return botao;
}

void draw_button(BUTTON *botao)
{   
    if(botao->estado)
    {
        al_draw_filled_rounded_rectangle(botao->x-3, botao->y-3, botao->x + botao->largura + 3, botao->y + botao->altura + 3, 
                                        2, 2, al_map_rgb(255, 255, 255));
        al_draw_filled_rounded_rectangle(botao->x, botao->y, botao->x + botao->largura, botao->y + botao->altura, 2, 2, botao->cor);
        al_draw_text(botao->fonte, al_map_rgb(0, 0, 0), botao->x + botao->padding_x + 1, botao->y + botao->padding_y, 0, botao->texto);
        al_draw_text(botao->fonte, al_map_rgb(255, 255, 255), botao->x + botao->padding_x, botao->y + botao->padding_y, 0, botao->texto);
    }    
    else
    {
        al_draw_filled_rounded_rectangle(botao->x, botao->y, botao->x + botao->largura, botao->y + botao->altura, 2, 2, botao->cor);
        al_draw_text(botao->fonte, al_map_rgb(0, 0, 0), botao->x + botao->padding_x + 1, botao->y + botao->padding_y, 0, botao->texto);
        al_draw_text(botao->fonte, al_map_rgb(255, 255, 255), botao->x + botao->padding_x, botao->y + botao->padding_y, 0, botao->texto);
    }
}

void destroi_button(BUTTON * botao)
{
    free(botao);
}


void draw_menu(BUTTON ** botoes, int tamanho_botoes, DESIGN ** designs, int tamanho_designs, ALLEGRO_FONT * fonte)
{
    
    for(int i = 0; i < tamanho_designs; i++)
    {
        draw_design(designs[i]);
    }

    for(int i = 0; i < tamanho_botoes; i++)
    {
        draw_button(botoes[i]);
    }
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 120, 220, 0, "SPACE INVADERS");
}

void draw_scenario(DESIGN ** designs, int tamanho_designs)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    for(int i = 0; i < tamanho_designs; i++)
    {
        draw_design(designs[i]);
    }
}

void draw_score(char * texto, int score, float x, float y, ALLEGRO_FONT *fonte)
{

    char buffer[20];
    char *nova_string;
    sprintf(buffer, "%d", score);

    int tamanho_nova_string = (int)strlen(texto) + (int)strlen(buffer);

    nova_string = (char*) malloc(tamanho_nova_string * sizeof(char));

    for(int i = 0; i < strlen(texto); i++)
    {
        nova_string[i] = texto[i];
    }

    for(int i = 0; i < strlen(buffer); i++)
    {
        nova_string[i+strlen(texto)] = buffer[i];
    }

    nova_string[tamanho_nova_string] = '\0';

    printf("%s\n", nova_string);


    al_draw_text(fonte, al_map_rgb(255, 255, 255), x, y, 0, nova_string);
}