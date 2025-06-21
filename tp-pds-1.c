#include <stdio.h>
#include<string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

// Determina a taxa de atualização do programa
const float FPS = 100;  

const int SCREEN_W = 960; // Largura da tela
const int SCREEN_H = 540; // Altura da tela

const int GRASS_H = 60;

const int NAVE_W = 100; 
const int NAVE_H = 50;

const int ALIEN_W = 50;
const int ALIEN_H = 45;

typedef struct NAVE
{
    float x;
    float vel;
    int dir, esq;
    ALLEGRO_COLOR cor;
}NAVE;

typedef struct ALIEN
{
    float x, y;
    float x_vel, y_vel;
    char *tipo;
    int linhas, colunas;
    int **desenho;
    ALLEGRO_COLOR cor;
}ALIEN;

void initNave(NAVE *nave)
{
    nave->x = SCREEN_W/2;
    nave->vel = 3;
    nave->dir = 0;
    nave->esq = 0;
    nave->cor = al_map_rgb(0, 0, 255);
}

void initAlien(ALIEN *alien,char *tipo)
{
    alien->x = 0;
    alien->y = 0;
    alien->x_vel = 1;
    alien->y_vel = ALIEN_H;
    alien->tipo = tipo;

    FILE * arq = NULL;

    if(!strcmp(tipo, "carangueijo"))
    {
        arq = fopen("crab.txt", "r");
    }
    else if(!strcmp(tipo, "polvo"))
    {
        arq = fopen("octopus.txt", "r");
    }
    else if(!strcmp(tipo, "molusco"))
    {
        arq = fopen("squid.txt", "r");
    }

    if(arq != NULL)
    {
        fscanf(arq, "%d", &alien->linhas);
        fscanf(arq, "%d", &alien->colunas);

        alien->desenho = (int**) malloc(alien->linhas * sizeof(int*));

        for(int i = 0; i < alien->linhas; i++)
        {
            alien->desenho[i] = (int*) malloc(alien->colunas * sizeof(int));
        }

        for(int i = 0; i < alien->linhas; i++)
        {
            for(int j = 0; j < alien->colunas; j++)
            {
                fscanf(arq, "%d", &alien->desenho[i][j]);
            }
        }
        fclose(arq);

        for(int i = 0; i < alien->linhas; i++)
        {
            for(int j = 0; j < alien->colunas; j++)
            {
                printf("%d ", alien->desenho[i][j]);
            }
            printf("\n");
        }
    }
    else
    {
        printf("Erro ao abrir arquivo\n");
        return ;
    }
    
    alien->cor = al_map_rgb(rand()%255, rand()%255, rand()%255);
}

void draw_scenario()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, SCREEN_H - GRASS_H, SCREEN_W, SCREEN_H, al_map_rgb(0, 240, 0));
}

void draw_nave(NAVE nave)
{
    float y_base = SCREEN_H - (GRASS_H/2);
    al_draw_filled_triangle(nave.x, y_base - NAVE_H, 
                            nave.x - (NAVE_W/2), y_base, 
                            nave.x + (NAVE_W/2), y_base, 
                            nave.cor);
}

void draw_alien(ALIEN alien)
{
   
    for(int i = 0; i < alien.linhas; i++)
    {
        for(int j = 0; j < alien.colunas; j++)
        {
            if(alien.desenho[i][j] == 1)
            {
                al_draw_filled_rectangle(alien.x + (j*5), alien.y + (i*5), 
                            alien.x + (j+1) * 5, alien.y + (i+1) * 5, 
                            alien.cor);
            }
            else if(alien.desenho[i][j] == 0)
            {
                al_draw_filled_rectangle(alien.x + (j*5), alien.y + (i*5), 
                            alien.x + (j+1) * 5, alien.y + (i+1) * 5, 
                            al_map_rgb(0, 0, 0));
            } 
        }
    }
    

    
}

void update_nave(NAVE *nave)
{
    if(nave->esq != nave->dir)
    {
        if(nave->esq == 1 && nave->x - (NAVE_W/2) - nave->vel >= 0)
        {
            nave->x -= nave->vel;
        }
        else if(nave->dir == 1 && nave->x + (NAVE_W/2) + nave->vel <= SCREEN_W)
        {
            nave->x += nave->vel;
        }
    }
}

void update_alien(ALIEN *alien)
{
    if(alien->x + ALIEN_W + alien->x_vel > SCREEN_W || alien->x + alien->x_vel < 0)
    {
        alien->y += ALIEN_H;
        alien->x_vel *= -1;
    }
    alien->x += alien->x_vel;
}

int colisao_alien_solo(ALIEN *alien)
{
    if(alien->y + ALIEN_H > SCREEN_H - GRASS_H)return 1;
    return 0;
}

void destroi_alien(ALIEN * alien)
{
    for(int i = 0; i < alien->linhas; i++)
    {
        free(alien->desenho[i]);
    }
    free(alien->desenho);
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


    NAVE nave;
    initNave(&nave);

    ALIEN alien;
    initAlien(&alien, "polvo");

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
            update_nave(&nave);
            update_alien(&alien);
            draw_nave(nave);
            draw_alien(alien);
            playing = !colisao_alien_solo(&alien);

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
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {

        }
        // Se o tipo de evento for o pressionar de uma tecla
        else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            // Imprime o código da tecla pressionada
            printf("\ncodigo tecla: %d", evento.keyboard.keycode);

            switch (evento.keyboard.keycode)
            {
                case ALLEGRO_KEY_A:
                    nave.esq = 1;
                    break;
                
                case ALLEGRO_KEY_D:
                    nave.dir = 1;
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
                    nave.esq = 0;
                    break;
                
                case ALLEGRO_KEY_D:
                    nave.dir = 0;
                    break;
                
                default:
                    break;
            }

        }
    }

    destroi_alien(&alien);
    al_destroy_display(display);
    al_destroy_event_queue(fila_de_eventos);
 
    al_rest(5);
	return 0;
}