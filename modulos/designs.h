#include "libs_e_tipos.h"

// 
DESIGN* init_design(float x, float y, char * tipo, float tamanho_px);

// 
void draw_design(DESIGN * design);

// 
void destroi_design(DESIGN * design);

// 
BUTTON * init_button(
    char * texto, int estado, float x, float y, float padding_x, float padding_y, 
    float altura,float largura, ALLEGRO_FONT * fonte, ALLEGRO_COLOR cor);

// 
void draw_button(BUTTON *botao);

// 
void destroi_button(BUTTON * botao);

// 
void draw_menu(BUTTON ** botoes, int tamanho_botoes, DESIGN ** designs, int tamanho_designs, ALLEGRO_FONT * fonte);

// 
void draw_scenario(DESIGN ** designs, int tamanho_designs);

// 
void draw_score(char * texto, int score, float x, float y, ALLEGRO_FONT *fonte);