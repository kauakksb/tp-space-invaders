#include <stdio.h>
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
        fprintf(stderr, "Failed to inicialize allegro!\n");
        return -1;
    }

    // Cria uma tela com dimensões SCREEN_W e SCREEN_H  
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        fprintf(stderr, "Failed to create display!\n");
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

    int playing = 1;

    // Inicia o temporizador
    al_start_timer(timer);
    while(playing)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_de_eventos, &evento);

        if(evento.type == ALLEGRO_EVENT_TIMER)
        {
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
        // Se o tipo de evento for um clique em um botão do mouse
        else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            // Imprime a coordenada da tela em que o mouse foi clicado
            printf("\nmouse clicado em: %d %d", evento.mouse.x, evento.mouse.y);
        }
        // Se o tipo de evento for o pressionar de uma tecla
        else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            // Imprime o código da tecla pressionada
            printf("\ncodigo tecla: %d", evento.keyboard.keycode);
            al_clear_to_color(al_map_rgb(rand()%255, rand()%255, rand()%255));
        }
    }
    
    al_destroy_display(display);
    al_destroy_event_queue(fila_de_eventos);
 
    al_rest(5);
	return 0;
}