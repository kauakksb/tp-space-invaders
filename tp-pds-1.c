#include "modulos/libs_e_tipos.h"
#include "modulos/alien.h"
#include "modulos/nave.h"
#include "modulos/util.h"
#include "modulos/jogador.h"
#include "modulos/designs.h"

// Determina a taxa de atualização do programa
#define FPS 100.00  

#define SCREEN_W 960 // Largura da tela
#define SCREEN_H 540 // Altura da tela
#define GRASS_H 60

typedef struct SESSION
{
    int num_players;
    int estado;
    int rodada;
    char *cenario;
    char *modo;
    char *level;
    JOGADOR ** jogadores;
}SESSION;

SESSION * init_session()
{
    SESSION * session = (SESSION*) malloc(sizeof(SESSION));
    session->estado = 1;
    session->rodada = 1;

    session->cenario = (char*) malloc(20 * sizeof(char));
    strcpy(session->cenario, "terra");

    session->modo = (char*) malloc(20 * sizeof(char));
    strcpy(session->modo, "menu-inicial");

    session->level = (char*) malloc(20 * sizeof(char));
    strcpy(session->level, "facil");

    session->jogadores = (JOGADOR**) malloc(2 * sizeof(JOGADOR*));

    return session;
}

void destroi_session(SESSION * session)
{
    for(int i = 0; i < session->num_players; i++)
    {
        destroi_jogador(session->jogadores[i]);
    }
    free(session->modo);
    free(session->level);
    free(session);
}


 
int main(int argc, char **argv){
	
    // Ponteiro para uma struct que representa a tela
	ALLEGRO_DISPLAY *display = NULL;
    // Ponteiro para uma struct que representa uma fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
    // Ponteiro para uma struct que representa o timer do programa
    ALLEGRO_TIMER *timer = NULL;

    SESSION *session = init_session();
	
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

    FILE * arq = fopen("recorde.txt", "r");
    int recorde;

    fscanf(arq, "%d", &recorde);

    fclose(arq);

    ALLEGRO_FONT *fonte_score = al_load_ttf_font("fonts/retro-gaming.ttf", 20, 0);
    if(!fonte_score)
    {
        fprintf(stderr, "Failed to load font");
        return -1;
    }

    ALLEGRO_FONT *fonte_titulo = al_load_ttf_font("fonts/retro-gaming.ttf", 70, 0);
    if(!fonte_titulo)
    {
        fprintf(stderr, "Failed to load font");
        return -1;
    }

    ALLEGRO_FONT *fonte_botoes = al_load_ttf_font("fonts/retro-gaming.ttf", 22, 0);
    if(!fonte_botoes)
    {
        fprintf(stderr, "Failed to load font");
        return -1;
    }
    
    DESIGN ** cenario_escolhido = (DESIGN**) malloc(13 * sizeof(DESIGN*));

    BUTTON ** botoes_menu = (BUTTON**) malloc(2 * sizeof(BUTTON*));
    BUTTON ** botoes_dificuldade = (BUTTON**) malloc(3 * sizeof(BUTTON*));
    BUTTON ** botoes_cenarios = (BUTTON**) malloc(2 * sizeof(BUTTON*));
    BUTTON ** botoes_naves = (BUTTON**) malloc(2 * sizeof(BUTTON*));

    DESIGN ** designs = (DESIGN**) malloc(10 * sizeof(DESIGN*));
    DESIGN ** design_gameplay = (DESIGN**) malloc(13 * sizeof(DESIGN*));
    DESIGN ** design_gameplay2 = (DESIGN**) malloc(13 * sizeof(DESIGN*));


    DESIGN * nave1 = init_design((SCREEN_W/2)- 65, 30, "nave1", 10);
    DESIGN * nave2 = init_design((SCREEN_W/2)- 65, 30, "nave2", 10);

    DESIGN * alien_menu = init_design(100, 130,"carangueijo", 5);
    DESIGN * alien_menu2 = init_design(700, 420,"polvo", 5);
    int vel_alien1 = 1;
    int vel_alien2 = -1;

    BUTTON *botao_um_jogador = init_button("1 Jogador", 1, (SCREEN_W/2) - 120, (SCREEN_H/2) + 60,
                                            50, 5 ,40, 240, fonte_botoes, al_map_rgb(255, 66, 65));
    
    BUTTON *botao_dois_jogadores = init_button("2 Jogadores", 0, (SCREEN_W/2) - 120, (SCREEN_H/2) + 115, 
                                                35, 5, 40, 240, fonte_botoes, al_map_rgb(255, 66, 65));

    botoes_menu[0] = botao_um_jogador;
    botoes_menu[1] = botao_dois_jogadores;

    BUTTON *botao_facil = init_button("Fácil", 1, (SCREEN_W/2) - 120, (SCREEN_H/2) + 60,
                                        85, 6, 40, 240, fonte_botoes, al_map_rgb(255, 66, 65));
    
    BUTTON *botao_normal = init_button("Normal", 0, (SCREEN_W/2) - 120, (SCREEN_H/2) + 115, 
                                        75, 6, 40, 240, fonte_botoes, al_map_rgb(255, 66, 65));

    BUTTON *botao_dificil = init_button("Difícil", 0, (SCREEN_W/2) - 120, (SCREEN_H/2) + 170, 
                                        80, 6, 40, 240, fonte_botoes, al_map_rgb(255, 66, 65));

    botoes_dificuldade[0] = botao_facil;
    botoes_dificuldade[1] = botao_normal;
    botoes_dificuldade[2] = botao_dificil;

    BUTTON *botao_cenario_terra = init_button("Terra", 1, (SCREEN_W/2) - 120, (SCREEN_H/2) + 60,
                                        85, 6, 40, 240, fonte_botoes, al_map_rgb(255, 66, 65));
    
    BUTTON *botao_cenario_marte = init_button("Marte", 0, (SCREEN_W/2) - 120, (SCREEN_H/2) + 115, 
                                        85, 6, 40, 240, fonte_botoes, al_map_rgb(255, 66, 65));

    botoes_cenarios[0] = botao_cenario_terra;
    botoes_cenarios[1] = botao_cenario_marte;

    BUTTON *botao_nave1 = init_button("Nave 1", 1, (SCREEN_W/2) - 120, (SCREEN_H/2) + 60,
                                        85, 6, 40, 240, fonte_botoes, al_map_rgb(255, 66, 65));
    
    BUTTON *botao_nave2 = init_button("Nave 2", 0, (SCREEN_W/2) - 120, (SCREEN_H/2) + 115, 
                                        85, 6, 40, 240, fonte_botoes, al_map_rgb(255, 66, 65));

    botoes_naves[0] = botao_nave1;
    botoes_naves[1] = botao_nave2;

    // DESIGN **estrelas = (DESIGN**) malloc(8 * sizeof(DESIGN*));
    float x[8] = {100, 220, 500, 830, 290, 600, 790, 870};
    float y[8] = {90, 200, 130, 40, 450, 360, 350, 270};
    for(int i = 0; i < 8; i++)
    {
        if(i % 2 == 0)
        {
            designs[i] = init_design(x[i],y[i], "star1", 1);
            design_gameplay[i] = designs[i];
            design_gameplay2[i] = designs[i];
        }
        else
        {
            designs[i] = init_design(x[i],y[i], "star2", 1);
            design_gameplay[i] = designs[i];
            design_gameplay2[i] = designs[i];
        }
    }
    designs[8] = alien_menu;
    designs[9] = alien_menu2;


    DESIGN * asteroide = init_design(SCREEN_W - 150, SCREEN_H - 100, "asteroide", 10);
    DESIGN * asteroide2 = init_design(0, SCREEN_H - 100, "asteroide2", 10);

    DESIGN * asteroide3 = init_design((SCREEN_W/2), SCREEN_H - 50, "asteroide2", 5);
    DESIGN * asteroide4 = init_design((SCREEN_W/2)- 50, SCREEN_H - 50, "asteroide", 5);

    DESIGN * terra = init_design(90, (SCREEN_H/2)-10, "terra", 7);
    DESIGN * marte = init_design(100, (SCREEN_H/2), "marte", 5);
    
    design_gameplay[8] = asteroide;
    design_gameplay[9] = asteroide2;
    design_gameplay[10] = asteroide3;
    design_gameplay[11] = asteroide4;
    design_gameplay[12] = terra;

    design_gameplay2[8] = asteroide;
    design_gameplay2[9] = asteroide2;
    design_gameplay2[10] = asteroide3;
    design_gameplay2[11] = asteroide4;
    design_gameplay2[12] = marte;

    int tamanho_esq = 10; // Quantidade aliens em um esquadrão
    int num_esq = 10; // Número de esquadrões
    GRUPO_ALIENS ** aliens = cria_esquadroes(tamanho_esq, num_esq, 30, 20);

    int playing = 0;
    int menu = 1;
    int dificuldade = 0;
    int cenario = 0;
    int selecao_nave = 0;

    printf("\nInicialização concluída\n");
    // Inicia o temporizador
    al_start_timer(timer);

    while(session->estado)
    {
        ALLEGRO_EVENT evento;
        
        if(!strcmp(session->modo, "menu-inicial"))
        {
            menu = 1;
            while(menu)
            {
                al_wait_for_event(fila_de_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_TIMER)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    draw_menu(botoes_menu, 2, designs, 10, fonte_titulo);

                    if(alien_menu->x + (alien_menu->tamanho_px * alien_menu->colunas) + vel_alien1 > 650 || alien_menu->x + vel_alien1 < 100)
                    {
                        vel_alien1 *= -1;
                    }
                    alien_menu->x += vel_alien1;

                    if(alien_menu2->x + (alien_menu2->tamanho_px * alien_menu2->colunas) + vel_alien2 > 800 || alien_menu2->x + vel_alien2 < 200)
                    {
                        vel_alien2 *= -1;
                    }
                    alien_menu2->x += vel_alien2;
                    printf("\nmenu concluído\n");

                    // Atualiza a tela (quando houver algo para mostrar)
                    al_flip_display();
                }
                else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    
                }
                else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    menu = 0;
                    session->estado = 0;
                }
                else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    switch (evento.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_ENTER:
                        if(botao_um_jogador->estado)
                        {
                            session->num_players = 1;
                        }
                        else
                        {
                            session->num_players = 2;
                        }
                        strcpy(session->modo, "dificuldade");
                        menu = 0;
                        break;
                    
                    case ALLEGRO_KEY_DOWN:
                        botao_dois_jogadores->estado = (botao_dois_jogadores->estado + 1) & 1;
                        botao_um_jogador->estado = (botao_um_jogador->estado + 1) & 1;
                        break;
                    
                    case ALLEGRO_KEY_UP:
                        botao_dois_jogadores->estado = (botao_dois_jogadores->estado + 1) & 1;
                        botao_um_jogador->estado = (botao_um_jogador->estado + 1) & 1;
                        break;
                    default:
                        break;
                    }
                }   
            }
        }
        else if(!strcmp(session->modo, "dificuldade"))
        {
            dificuldade = 1;
            while(dificuldade)
            {
                al_wait_for_event(fila_de_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_TIMER)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    draw_menu(botoes_dificuldade, 3, designs, 10, fonte_titulo);

                    if(alien_menu->x + (alien_menu->tamanho_px * alien_menu->colunas) + vel_alien1 > 650 || alien_menu->x + vel_alien1 < 100)
                    {
                        vel_alien1 *= -1;
                    }
                    alien_menu->x += vel_alien1;

                    if(alien_menu2->x + (alien_menu2->tamanho_px * alien_menu2->colunas) + vel_alien2 > 800 || alien_menu2->x + vel_alien2 < 200)
                    {
                        vel_alien2 *= -1;
                    }
                    alien_menu2->x += vel_alien2;
                    printf("\nmenu concluído\n");

                    // Atualiza a tela (quando houver algo para mostrar)
                    al_flip_display();
                }
                else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    
                }
                else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    dificuldade = 0;
                    session->estado = 0;
                }
                else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    switch (evento.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_ENTER:
                        
                        strcpy(session->modo, "selecao-nave");
                        dificuldade = 0;
                        break;
                    
                    case ALLEGRO_KEY_DOWN:

                        if(botao_facil->estado)
                        {
                            botao_facil->estado = (botao_facil->estado + 1) & 1;
                            botao_normal->estado = (botao_normal->estado + 1) & 1;
                            strcpy(session->level, "normal");
                        }
                        else if(botao_normal->estado)
                        {
                            botao_normal->estado = (botao_normal->estado + 1) & 1;
                            botao_dificil->estado = (botao_dificil->estado + 1) & 1;
                            strcpy(session->level, "dificil");
                        }
                        else if(botao_dificil->estado)
                        {
                            botao_dificil->estado = (botao_dificil->estado + 1) & 1;
                            botao_facil->estado = (botao_facil->estado + 1) & 1;
                            strcpy(session->level, "facil");
                        }
                        break;
                    
                    case ALLEGRO_KEY_UP:
                        if(botao_facil->estado)
                        {
                            botao_facil->estado = (botao_facil->estado + 1) & 1;
                            botao_dificil->estado = (botao_dificil->estado + 1) & 1;
                            strcpy(session->level, "dificil");
                        }
                        else if(botao_normal->estado)
                        {
                            botao_normal->estado = (botao_normal->estado + 1) & 1;
                            botao_facil->estado = (botao_facil->estado + 1) & 1;
                            strcpy(session->level, "facil");
                        }
                        else if(botao_dificil->estado)
                        {
                            botao_dificil->estado = (botao_dificil->estado + 1) & 1;
                            botao_normal->estado = (botao_normal->estado + 1) & 1;
                            strcpy(session->level, "normal");
                        }
                        break;
                    default:
                        break;
                    }
                }   
            }
        }
        else if(!strcmp(session->modo, "selecao-nave"))
        {
            selecao_nave = 1;
            while(selecao_nave)
            {
                al_wait_for_event(fila_de_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_TIMER)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    draw_menu(botoes_naves, 2, designs, 10, fonte_titulo);

                    if(botao_nave1->estado)
                    {
                        draw_design(nave1);
                    }
                    else if(botao_nave2->estado)
                    {
                        draw_design(nave2);
                    }

                    if(alien_menu->x + (alien_menu->tamanho_px * alien_menu->colunas) + vel_alien1 > 650 || alien_menu->x + vel_alien1 < 100)
                    {
                        vel_alien1 *= -1;
                    }
                    alien_menu->x += vel_alien1;

                    if(alien_menu2->x + (alien_menu2->tamanho_px * alien_menu2->colunas) + vel_alien2 > 800 || alien_menu2->x + vel_alien2 < 200)
                    {
                        vel_alien2 *= -1;
                    }
                    alien_menu2->x += vel_alien2;
                    printf("\nmenu concluído\n");

                    // Atualiza a tela (quando houver algo para mostrar)
                    al_flip_display();
                }
                else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    
                }
                else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    selecao_nave = 0;
                    session->estado = 0;
                }
                else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    switch (evento.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_ENTER:
                        if(botao_nave1->estado)
                        {
                            if(session->num_players == 1)
                            {
                                session->jogadores[0] = initJogador("Jogador 1", 0, 3, "nv1");
                            }
                            else if(session->num_players == 2)
                            {
                                session->jogadores[0] = initJogador("Jogador 1", 0, 3, "nv1");
                                session->jogadores[1] = initJogador("Jogador 1", 1, 3, "nv2");
                            }
                        }
                        else if(botao_nave2->estado)
                        {
                            if(session->num_players == 1)
                            {
                                session->jogadores[0] = initJogador("Jogador 1", 0, 3, "nv2");
                            }
                            else if(session->num_players == 2)
                            {
                                session->jogadores[0] = initJogador("Jogador 1", 0, 3, "nv1");
                                session->jogadores[1] = initJogador("Jogador 1", 1, 3, "nv2");
                            }
                        }
                        strcpy(session->modo, "cenario");
                        selecao_nave = 0;
                        break;
                    
                    case ALLEGRO_KEY_DOWN:
                        botao_nave1->estado = (botao_nave1->estado + 1) & 1;
                        botao_nave2->estado = (botao_nave2->estado + 1) & 1;
                        break;
                    
                    case ALLEGRO_KEY_UP:
                        botao_nave1->estado = (botao_nave1->estado + 1) & 1;
                        botao_nave2->estado = (botao_nave2->estado + 1) & 1;
                        break;
                    default:
                        break;
                    }
                }   
            } 
        }
        else if(!strcmp(session->modo, "cenario"))
        {
            cenario = 1;
            while(cenario)
            {
                al_wait_for_event(fila_de_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_TIMER)
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    if(botao_cenario_terra->estado)
                    {
                        draw_menu(botoes_cenarios, 2, design_gameplay, 13, fonte_titulo);
                    }
                    else if(botao_cenario_marte->estado)
                    {
                        draw_menu(botoes_cenarios, 2, design_gameplay2, 13, fonte_titulo);
                    }

                    if(alien_menu->x + (alien_menu->tamanho_px * alien_menu->colunas) + vel_alien1 > 650 || alien_menu->x + vel_alien1 < 100)
                    {
                        vel_alien1 *= -1;
                    }
                    alien_menu->x += vel_alien1;

                    if(alien_menu2->x + (alien_menu2->tamanho_px * alien_menu2->colunas) + vel_alien2 > 800 || alien_menu2->x + vel_alien2 < 200)
                    {
                        vel_alien2 *= -1;
                    }
                    alien_menu2->x += vel_alien2;
                    printf("\nmenu concluído\n");

                    // Atualiza a tela (quando houver algo para mostrar)
                    al_flip_display();
                }
                else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    
                }
                else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    cenario = 0;
                    session->estado = 0;
                }
                else if(evento.type == ALLEGRO_EVENT_KEY_DOWN)
                {
                    switch (evento.keyboard.keycode)
                    {
                    case ALLEGRO_KEY_ENTER:
                        if(botao_cenario_terra->estado)
                        {
                            strcpy(session->cenario, "terra");
                        }
                        else if(botao_cenario_marte->estado)
                        {
                            strcpy(session->cenario, "marte");
                        }
                        strcpy(session->modo, "playing");
                        cenario = 0;
                        break;
                    
                    case ALLEGRO_KEY_DOWN:
                        botao_cenario_terra->estado = (botao_cenario_terra->estado + 1) & 1;
                        botao_cenario_marte->estado = (botao_cenario_marte->estado + 1) & 1;
                        break;
                    
                    case ALLEGRO_KEY_UP:
                        botao_cenario_terra->estado = (botao_cenario_terra->estado + 1) & 1;
                        botao_cenario_marte->estado = (botao_cenario_marte->estado + 1) & 1;
                        break;
                    default:
                        break;
                    }
                }   
            }
        }
        else if(!strcmp(session->modo, "playing"))
        {

            arq = fopen("recorde.txt", "r");
            fscanf(arq, "%d", &recorde);
            fclose(arq);
            int num_esq_draw;
            int num_alien_draw;
            playing = 1;
            if(!strcmp(session->level, "facil"))
            {
                num_esq_draw = 4;
                num_alien_draw = 5;
            }
            else if(!strcmp(session->level, "normal"))
            {
                num_esq_draw = 4;
                num_alien_draw = 6;
            }
            else if(!strcmp(session->level, "dificil"))
            {
                num_esq_draw = 5;
                num_alien_draw = 7;
            }
            define_aliens_used(aliens, tamanho_esq, num_esq, num_alien_draw, num_esq_draw);
            
            if(!strcmp(session->cenario, "terra"))
            {
                cenario_escolhido = design_gameplay;
            }
            else if(!strcmp(session->cenario, "marte"))
            {
                cenario_escolhido = design_gameplay2;
            }

            while(playing)
            {
                al_wait_for_event(fila_de_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_TIMER)
                {
                    draw_scenario(cenario_escolhido, 13);
                    
                    draw_score("<SCORE 1>:", session->jogadores[0]->score, 10, 10, fonte_score);
                    draw_score("<HIGH SCORE>:", recorde, (SCREEN_W/2) - 110, 10, fonte_score);
                    draw_score("<SCORE 2>:", session->jogadores[0]->score, SCREEN_W - 180, 10, fonte_score);
                    update_nave(session->jogadores[0]->nave);

                    for(int i = 0; i < num_esq; i++)
                    {
                        update_grupo_aliens(aliens[i]);
                    }

                    update_tiro(&session->jogadores[0]->nave->municao, session->jogadores[0]->nave->x);
                    draw_nave(session->jogadores[0]->nave);

                    for(int i = 0; i < num_esq_draw; i++)
                    {
                        imprime_grupo_aliens(aliens[i]->esquadrao, num_alien_draw);
                    }

                    draw_tiro(session->jogadores[0]->nave->municao);

                    if(colisao_tiro(&session->jogadores[0]->nave->municao, session->jogadores[0]->nave->x, session->jogadores[0]->nave->y,aliens, tamanho_esq, num_esq))
                    {
                        session->jogadores[0]->score++;
                        if(session->jogadores[0]->contador_pw < 10)
                        {
                            session->jogadores[0]->contador_pw++;
                        }
                    }
                    if(session->jogadores[0]->contador_pw == 10)
                    {
                        session->jogadores[0]->nave->municao.y_vel = 20;
                    }

                    if(session->jogadores[0]->num_tiros_pw == 0)
                    {
                        session->jogadores[0]->nave->municao.y_vel = 4;
                        session->jogadores[0]->num_tiros_pw = 10;
                        session->jogadores[0]->contador_pw = 0;
                    }

                    if(verifica_colisao_esquadrao(aliens, num_esq))
                    {
                        // strcpy(session->modo, "fim_de_jogo");
                        reset_alien(aliens, num_esq_draw, num_alien_draw, 30, 30);
                        reset_nave(session->jogadores[0]->nave);
                        strcpy(session->modo, "menu-inicial");
                        session->rodada = 0;
                        playing = 0;
                    }
                    else if (rodada_acabou(aliens, num_esq))
                    {
                        // strcpy(session->modo, "fim_de_jogo");
                        reset_alien(aliens, num_esq_draw, num_alien_draw, 30, 30);
                        reset_nave(session->jogadores[0]->nave);
                        if(session->rodada >= 5)
                        {
                            strcpy(session->modo, "menu-inicial");
                            if(guarda_recorde(session->jogadores[0]->score, recorde))
                            {
                                strcpy(session->modo, "menu-inicial");
                            }
                            session->rodada = 0;
                            playing = 0;
                            break;
                        }
                        session->rodada++;

                          
                    }
                    else if(alien_proximo_nave(session->jogadores[0]->nave, aliens, tamanho_esq, num_esq))
                    {
                        reset_alien(aliens, num_esq_draw, num_alien_draw, 30, 30);
                        reset_nave(session->jogadores[0]->nave);
                        strcpy(session->modo, "menu-inicial");
                        session->rodada = 0;
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
                    session->estado = 0;
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
                            session->jogadores[0]->nave->esq = 1;
                            break;
                        
                        case ALLEGRO_KEY_D:
                            printf("Tecla D\n");
                            session->jogadores[0]->nave->dir = 1;
                            break;

                        case ALLEGRO_KEY_SPACE:
                            printf("Tecla Espaço\n");
                            session->jogadores[0]->nave->municao.estado = 1;
                            if(session->jogadores[0]->contador_pw == 10)
                            {
                                session->jogadores[0]->num_tiros_pw--;
                            }
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
                            session->jogadores[0]->nave->esq = 0;
                            break;
                        
                        case ALLEGRO_KEY_D:
                            session->jogadores[0]->nave->dir = 0;
                            break;
                        
                        default:
                            break;
                    }
                }
            }
        }
    }

    for(int i = 0; i < 8; i++)
    {
        destroi_design(designs[i]);
    }
    destroi_design(nave1);
    destroi_design(nave2);
    destroi_design(alien_menu);
    destroi_design(alien_menu2);
    destroi_design(asteroide);
    destroi_design(asteroide2);
    destroi_design(asteroide3);
    destroi_design(asteroide4);
    destroi_design(terra);
    destroi_design(marte);
    free(designs);
    free(design_gameplay);
    free(design_gameplay2);
    destroi_button(botao_um_jogador);
    destroi_button(botao_dois_jogadores);
    destroi_button(botao_facil);
    destroi_button(botao_normal);
    destroi_button(botao_dificil);
    destroi_button(botao_cenario_terra);
    destroi_button(botao_cenario_marte);
    destroi_button(botao_nave1);
    destroi_button(botao_nave2);
    free(botoes_menu);
    free(botoes_dificuldade);
    free(botoes_cenarios);
    free(botoes_naves);
    destroi_esquadrao(aliens, tamanho_esq, num_esq);
    al_destroy_font(fonte_score);
    al_destroy_font(fonte_titulo);
    al_destroy_font(fonte_botoes);
    destroi_session(session);
    al_destroy_display(display);
    al_destroy_event_queue(fila_de_eventos);
    al_destroy_timer(timer);
    al_rest(1);
	return 0;
}