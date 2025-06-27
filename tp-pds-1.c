#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "modulos/alien.h"
#include "modulos/nave.h"
#include "modulos/util.h"

// Determina a taxa de atualização do programa
#define FPS 100.00  

#define SCREEN_W 960 // Largura da tela
#define SCREEN_H 540 // Altura da tela
#define GRASS_H 60

#define NAVE_W 100 
#define NAVE_H 50

#define PONTA_NAVE SCREEN_H - (GRASS_H/2) - NAVE_H

typedef struct STAR
{
    int **desenho;
    int linhas, colunas;
    float x, y;
}STAR;


STAR* init_star(float x, float y)
{
    STAR * estrela = (STAR*) malloc(sizeof(STAR));
    estrela->x = x;
    estrela->y = y;

    estrela->desenho = guarda_desenho(&estrela->linhas, &estrela->colunas, "designs/star.txt");

    return estrela;
}

void draw_star(STAR * estrela)
{
    for(int i = 0; i < estrela->linhas; i++)
    {
        for(int j = 0; j < estrela->colunas; j++)
        {
            if(estrela->desenho[i][j] == 1)
            {
                al_draw_filled_rectangle(estrela->x + (j), estrela->y + (i),
                                        estrela->x + (j+1) , estrela->y +(i+1), 
                                        al_map_rgb(255, 255, 255));
            }
            else if(estrela->desenho[i][j] == 2)
            {
                al_draw_filled_rectangle(estrela->x + (j), estrela->y + (i),
                                        estrela->x + (j+1), estrela->y + (i+1), 
                                        al_map_rgb(255, 253, 123));
            }
            else if (estrela->desenho[i][j] == 3)
            {
                al_draw_filled_rectangle(estrela->x + (j), estrela->y + (i),
                                        estrela->x + (j+1), estrela->y +(i+1), 
                                        al_map_rgb(199, 199, 199));
            }
        }
    }
            
}

void destroi_star(STAR * estrela)
{
    for(int i = 0; i < estrela->linhas; i++)
    {
        free(estrela->desenho[i]);
    }
    free(estrela->desenho);
    free(estrela);
}

void draw_menu()
{

}

int colisao_tiro(TIRO * municao, float novo_x, GRUPO_ALIENS ** aliens, int tamanho_esq, int n)
{

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < tamanho_esq; j++)
        {
            if(aliens[i]->esquadrao[j]->estado == 1)
            {
                int x1 = aliens[i]->esquadrao[j]->x;
                int y1 = aliens[i]->esquadrao[j]->y;
                int x2 = aliens[i]->esquadrao[j]->x + aliens[i]->esquadrao[j]->largura;
                int y2 = aliens[i]->esquadrao[j]->y + aliens[i]->esquadrao[j]->altura;
                if (municao->x + municao->raio > x1 && municao->x + municao->raio < x2)
                {
                    if(municao->y + municao->raio > y1 && municao->y + municao->raio < y2)
                    {
                        printf("colidiu");
                        municao->estado = 0;
                        municao->y = PONTA_NAVE;
                        municao->x = novo_x;
                        aliens[i]->esquadrao[j]->estado = 0;
                        return 1;
                    }
                }
                else if(municao->x - municao->raio > x1 && municao->x - municao->raio < x2)
                {
                    if(municao->y - municao->raio > y1 && municao->y - municao->raio < y2)
                    {
                        printf("colidiu");
                        municao->estado = 0;
                        municao->y = PONTA_NAVE;
                        municao->x = novo_x;
                        aliens[i]->esquadrao[j]->estado = 0;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void draw_scenario()
{

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, SCREEN_H - GRASS_H, SCREEN_W, SCREEN_H, al_map_rgb(0, 240, 0));



}

void draw_score(ALLEGRO_FONT *fonte, int score)
{
    char str[] = "<SCORE>:";
    char buffer[20];
    char *nova_string;
    sprintf(buffer, "%d", score);

    int tamanho_nova_string = (int)strlen(str) + (int)strlen(buffer);

    nova_string = (char*) malloc(tamanho_nova_string * sizeof(char));

    for(int i = 0; i < strlen(str); i++)
    {
        nova_string[i] = str[i];
    }

    for(int i = 0; i < strlen(buffer); i++)
    {
        nova_string[i+strlen(str)] = buffer[i];
    }

    nova_string[tamanho_nova_string] = '\0';

    printf("%s\n", nova_string);


    al_draw_text(fonte, al_map_rgb(255, 255, 255), 10, 10, 0, nova_string);
}

int rodada_acabou(GRUPO_ALIENS **aliens, int n)
{
    int resultado = 0;
    for(int i = 0; i < n; i++)
    {
        if(aliens[i]->estado)resultado++;
    }

    if(resultado == 0)return 1;
    return 0;
}
 
int main(int argc, char **argv){
	
    // Ponteiro para uma struct que representa a tela
	ALLEGRO_DISPLAY *display = NULL;
    // Ponteiro para uma struct que representa uma fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
    // Ponteiro para uma struct que representa o timer do programa
    ALLEGRO_TIMER *timer = NULL;
	
    // Função que inicializa o Allegro e reporta um erro se não conseguir realizar a operação
    if(!al_init())
    {
        // stderr --> é uma forma de refenciar a tela
        fprintf(stderr, "Failed to initialize allegro!\n");
        return -1;
    }

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    // Cria uma tela com dimensões SCREEN_W e SCREEN_H  
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        fprintf(stderr, "Failed to create display!\n");
        return -1;
    }
    
    // Inicia o módulo das primitivas do Allegro
    if(!al_init_primitives_addon())
    {
        fprintf(stderr, "Failed to initialize primitives!\n");
        return -1;
    }

    if(!al_init_font_addon())
    {
        fprintf(stderr, "Failed to initialize fonts");
        return -1;
    }

    if(!al_init_ttf_addon())
    {
        fprintf(stderr, "Failed to initialize trueTypeFonts");
        return -1;
    }

    // Cria o temporizador/relógio do programa
    timer = al_create_timer(1.0/FPS);
    if(!timer)
    {
        fprintf(stderr, "Failed to create timer!\n");
        return -1;
    }

    // Cria uma fila de eventos
    fila_de_eventos = al_create_event_queue();
    if(!fila_de_eventos)
    {
        fprintf(stderr, "Failed to create event queue!\n");
        al_destroy_display(display);
        return -1;
    }

    // Instala o teclado
    if(!al_install_keyboard())
    {
        fprintf(stderr, "Failed to install keyboard!\n");
        return -1;
    }

    // Instala o mouse
    if(!al_install_mouse())
    {
        fprintf(stderr, "Failed to install mouse!\n");
        return -1;
    }

    // Registra na fila os eventos de tela (ex: clicar no X da janela)
    al_register_event_source(fila_de_eventos, al_get_display_event_source(display)); 

    // Registra na fila os eventos de teclado (ex: pressionar uma tecla)
    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source()); 

    // Registra na fila os eventos de mouse (ex: clicar em um botão do mouse)
    al_register_event_source(fila_de_eventos, al_get_mouse_event_source()); 

    // Registra na fila os eventos de tempo: quando o tempo altera de t para t+1
    al_register_event_source(fila_de_eventos, al_get_timer_event_source(timer));


    NAVE * nave = initNave("nv2");

    printf("nave x: %f" , nave->x);
    printf("nave dir: %d", nave->dir);
    printf("nave esq: %d", nave->esq);
    printf("nave vel: %f", nave->vel);

    STAR * estrela1 = init_star(600, 300);

    
    int tamanho_esq = 6; // Quantidade aliens em um esquadrão
    int num_esq = 4; // Número de esquadrões
    GRUPO_ALIENS ** aliens = cria_esquadroes(tamanho_esq, num_esq, 30, 30);

    ALLEGRO_FONT *fonte = al_load_ttf_font("fonts/retro-gaming.ttf", 20, 0);
    if(!fonte)
    {
        fprintf(stderr, "Failed to load font");
        return -1;
    }

    int score = 0;
    int playing = 1;

    // Inicia o temporizador
    al_start_timer(timer);
    while(playing)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_de_eventos, &evento);

        if(evento.type == ALLEGRO_EVENT_TIMER)
        {

            draw_scenario();
            draw_score(fonte, score);
            draw_star(estrela1);

            printf("enderco de nave: %p", &nave);

            printf("nave esq: %d", nave->esq);
            printf("nave dir: %d", nave->dir);
            update_nave(nave);

            for(int i = 0; i < num_esq; i++)
            {
                update_grupo_aliens(aliens[i]);
            }

            update_tiro(&nave->municao, nave->x);
            draw_nave(nave);

            for(int i = 0; i < num_esq; i++)
            {
                imprime_grupo_aliens(aliens[i]->esquadrao, tamanho_esq);
            }

            draw_tiro(nave->municao);

            if(colisao_tiro(&nave->municao, nave->x,aliens, tamanho_esq, num_esq))score++;
            printf("Score: %d", score);


            if(verifica_colisao_esquadrao(aliens, num_esq))
            {
                playing = 0;
            }
            else if (rodada_acabou(aliens, num_esq))
            {
                playing = 0;
            }
            

            // Atualiza a tela (quando houver algo para mostrar)
            al_flip_display();

            if(al_get_timer_count(timer) % (int)FPS == 0)
            {
                printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
            }

        }
        // Se o tipo de evento for o fechamento da tela
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            playing = 0;
        }
        // Se o tipo de evento for o pressionar de uma tecla
        else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            // Imprime o código da tecla pressionada
            printf("\ncodigo tecla: %d", evento.keyboard.keycode);

            switch (evento.keyboard.keycode)
            {
                case ALLEGRO_KEY_A:
                    printf("Tecla A\n");
                    nave->esq = 1;
                    printf("nave esq: %d", nave->esq);
                    break;
                
                case ALLEGRO_KEY_D:
                    printf("Tecla D\n");
                    nave->dir = 1;
                    printf("nave dir: %d", nave->dir);
                    break;

                case ALLEGRO_KEY_SPACE:
                    printf("Tecla Espaço\n");
                    nave->municao.estado = 1;
                    break;
                default:
                    break;
            }
        }
        // Se o tipo de evento for soltar uma tecla
        else if(evento.type == ALLEGRO_EVENT_KEY_UP)
        {
            // Imprime o código da tecla liberada
            printf("\ncodigo tecla: %d", evento.keyboard.keycode);
            switch (evento.keyboard.keycode)
            {
                case ALLEGRO_KEY_A:
                    nave->esq = 0;
                    break;
                
                case ALLEGRO_KEY_D:
                    nave->dir = 0;
                    break;
                
                default:
                    break;
            }
        }
    }

    destroi_star(estrela1);
    destroi_nave(nave);
    destroi_esquadrao(aliens, tamanho_esq, num_esq);
    al_destroy_font(fonte);
    al_destroy_display(display);
    al_destroy_event_queue(fila_de_eventos);
    al_destroy_timer(timer);
 
    al_rest(1);
	return 0;
}