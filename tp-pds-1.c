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

const int PONTA_NAVE = SCREEN_H - (GRASS_H/2) - NAVE_H;

typedef struct TIRO
{
    int estado;
    float x, y;
    float y_vel;
    float raio;
    ALLEGRO_COLOR cor;
}TIRO;

typedef struct NAVE
{
    float x;
    float vel;
    int dir, esq;
    ALLEGRO_COLOR cor;
    TIRO municao;
}NAVE;


typedef struct ALIEN
{
    char *tipo;
    int **desenho;
    int id, grupo;
    int linhas, colunas;
    int altura, largura;
    float x, y;
    float x_vel, y_vel;
    ALLEGRO_COLOR cor;
}ALIEN;


typedef struct GRUPO_ALIENS
{
    ALIEN **esquadrao;
    int id_grupo;
    char *tipo;
    float x, y;
    float espacamento_x;
    float espacamento_y;
    int altura, largura;
    int num_aliens;
}GRUPO_ALIENS;

void initNave(NAVE *nave)
{
    nave->x = SCREEN_W/2;
    nave->vel = 3;
    nave->dir = 0;
    nave->esq = 0;
    nave->cor = al_map_rgb(0, 0, 255);

    nave->municao.estado = 0;
    nave->municao.raio = 5;
    nave->municao.x = nave->x;
    nave->municao.y = PONTA_NAVE;
    nave->municao.y_vel = 4;
    nave->municao.cor = al_map_rgb(255, 0, 0);
}

void draw_nave(NAVE nave)
{
    float y_base = SCREEN_H - (GRASS_H/2);
    al_draw_filled_triangle(nave.x, y_base - NAVE_H, 
                            nave.x - (NAVE_W/2), y_base, 
                            nave.x + (NAVE_W/2), y_base, 
                            nave.cor);
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

        if(nave->municao.estado == 0)
        {
            nave->municao.x = nave->x;
        }
    }
}

void draw_tiro(TIRO municao)
{
    if(municao.estado == 1)
    {
        al_draw_filled_circle(municao.x, municao.y, municao.raio, municao.cor);
    }
}

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

void colisao_tiro(TIRO * municao, float novo_x, GRUPO_ALIENS ** aliens, int tamanho_esq, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < tamanho_esq; j++)
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
                }
            }
            
        }
    }
}

// Instancia uma struct de ALIEN e retorna seu endereço de memória (o seu ponteiro)
ALIEN *initAlien(char *tipo, int id, int grupo, float espacamento_x, float espacamento_y)
{
    // Criando ponteiro de ALIEN
    ALIEN *alien;

    // Alocando espaço para a struct ALIEN
    alien = (ALIEN*) malloc(sizeof(ALIEN));

    // Armazenando id e grupo do alien
    alien->id = id;
    alien->grupo = grupo;

    // Alocando espaço para o tipo do alien e guardando o tipo
    alien->tipo = (char*) malloc(strlen(tipo) * sizeof(char));
    strcpy(alien->tipo, tipo);

    // Declarando variável que será usada para abrir o arquivo do desenho do alien
    FILE * arq = NULL;

    // Verificando qual o tipo do alien
    if(!strcmp(alien->tipo, "carangueijo"))
    {
        arq = fopen("crab.txt", "r");
    }
    else if(!strcmp(alien->tipo, "polvo"))
    {
        arq = fopen("octopus.txt", "r");
    }
    else if(!strcmp(alien->tipo, "molusco"))
    {
        arq = fopen("squid.txt", "r");
    }

    // Verificando se o arquivo é nulo e caso contrário, realiza a leitura dos dados
    if(arq != NULL)
    {
        // Lê quantas linhas e colunas compoem o desenho do alien
        fscanf(arq, "%d", &alien->linhas);
        fscanf(arq, "%d", &alien->colunas);

        // Guarda as medidas de altura e largura baseada no desenho
        alien->altura = 5 * alien->linhas;
        alien->largura = 5 * alien->colunas; 

        // Aloca espaço para a matriz do desenho do alien 
        alien->desenho = (int**) malloc(alien->linhas * sizeof(int*));
        for(int i = 0; i < alien->linhas; i++)
        {
            alien->desenho[i] = (int*) malloc(alien->colunas * sizeof(int));
        }

        // Guarda os valores de cada entrada da matriz
        for(int i = 0; i < alien->linhas; i++)
        {
            for(int j = 0; j < alien->colunas; j++)
            {
                fscanf(arq, "%d", &alien->desenho[i][j]);
            }
        }
        fclose(arq); // Fecha o arquivo da matriz do desenho

        // Imprime os valores da matriz no terminal para verificar de a leitura foi realizada corretamente
        for(int i = 0; i < alien->linhas; i++)
        {
            for(int j = 0; j < alien->colunas; j++)
            {
                printf("%d ", alien->desenho[i][j]);
            }
            printf("\n");
        }
    }
    // Retorna 0 em caso de erro ao abrir o arquivo e imprime o erro no terminal
    else
    {
        printf("Erro ao abrir arquivo\n");
        return 0;
    }

    // Armazena as posições iniciais do alien baseado em no grupo ao qual o alien pertence e o seu id no grupo
    alien->x = espacamento_x + (alien->largura + espacamento_x) * alien->id;
    alien->y = espacamento_y + (alien->altura + espacamento_y) * alien->grupo; 

    // Determina as velocidades de movitação do alien na vertical e na horizontal
    alien->x_vel = 1;
    alien->y_vel = alien->altura;
    
    // Armazena uma cor para o alien
    alien->cor = al_map_rgb(rand()%255, rand()%255, rand()%255);

    // Retorna o endereço de memória da struct
    return alien;
}

GRUPO_ALIENS *init_grupo_aliens(int n, int id_grupo, char * tipo, float espacamento_x, float espacamento_y)
{
    GRUPO_ALIENS *grupo_aliens = (GRUPO_ALIENS*) malloc(sizeof(GRUPO_ALIENS));
    
    grupo_aliens->esquadrao = (ALIEN**) malloc(n * sizeof(ALIEN*));
    grupo_aliens->id_grupo = id_grupo;
    grupo_aliens->num_aliens = n;
    grupo_aliens->espacamento_x = espacamento_x;
    grupo_aliens->espacamento_y = espacamento_y;

    grupo_aliens->tipo = (char*) malloc(strlen(tipo) * sizeof(char));
    strcpy(grupo_aliens->tipo, tipo);

    for(int i = 0; i < grupo_aliens->num_aliens; i++)
    {
        grupo_aliens->esquadrao[i] = initAlien(tipo, i, grupo_aliens->id_grupo, 
                                                grupo_aliens->espacamento_x, 
                                                grupo_aliens->espacamento_y);
    }

    grupo_aliens->x = grupo_aliens->esquadrao[0]->x;
    grupo_aliens->y = grupo_aliens->esquadrao[0]->y;

    int i = 0;
    grupo_aliens->largura = 0;
    while(1)
    {
        grupo_aliens->largura += grupo_aliens->esquadrao[i]->largura;
        if(i == grupo_aliens->num_aliens - 1)break;
        grupo_aliens->largura += espacamento_x;
        i++;
    }
    grupo_aliens->altura = grupo_aliens->esquadrao[i]->altura;

    printf("Largura esquadrao: %d\n", grupo_aliens->largura);
    printf("Altura esquadrao: %d\n", grupo_aliens->altura);

    grupo_aliens->altura = grupo_aliens->esquadrao[0]->altura;

    return grupo_aliens;
}

GRUPO_ALIENS ** cria_esquadroes(int tamanho_esq, int n)
{
    GRUPO_ALIENS** aliens = (GRUPO_ALIENS **) malloc(n * sizeof(GRUPO_ALIENS*));
    for(int i = 0; i < n; i++)
    {
        aliens[i] = init_grupo_aliens(tamanho_esq, i, "polvo", 30, 30);
    }

    return aliens;
}

void draw_alien(ALIEN *alien)
{
   
    for(int i = 0; i < alien->linhas; i++)
    {
        for(int j = 0; j < alien->colunas; j++)
        {
            if(alien->desenho[i][j] == 1)
            {
                al_draw_filled_rectangle(alien->x + (j*5), alien->y + (i*5), 
                            alien->x + (j+1) * 5, alien->y + (i+1) * 5, 
                            alien->cor);
            }
        }
    }
}

void imprime_grupo_aliens(ALIEN** grupo, int n)
{
    for(int i = 0; i < n; i++)
    {
        draw_alien(grupo[i]);
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

void update_grupo_aliens(GRUPO_ALIENS * grupo_aliens)
{
    if((grupo_aliens->x + grupo_aliens->largura + grupo_aliens->esquadrao[0]->x_vel) >= (SCREEN_W - 20) ||
        (grupo_aliens->x + grupo_aliens->esquadrao[0]->x_vel) <= 20)
    {
        for(int i = 0; i < grupo_aliens->num_aliens; i++)
        {
            grupo_aliens->esquadrao[i]->y += grupo_aliens->altura;
            grupo_aliens->esquadrao[i]->x_vel *= -1;
        }
        
    }
    
    for(int i = 0; i < grupo_aliens->num_aliens; i++)
    {
        update_alien(grupo_aliens->esquadrao[i]);
    }
    grupo_aliens->x += grupo_aliens->esquadrao[0]->x_vel;
    
}

int colisao_alien_solo(ALIEN *alien)
{
    if(alien->y + ALIEN_H > SCREEN_H - GRASS_H)return 1;
    return 0;
}

void destroi_alien(ALIEN * alien)
{
    // Liberando memória alocada para cada linha da matriz desenho (int*)
    for(int i = 0; i < alien->linhas; i++)
    {
        free(alien->desenho[i]);
    }
    // Liberando memória alocada para toda a matriz (int**)
    free(alien->desenho);
    // Liberando memória alocada para o tipo do alien (char*)
    free(alien->tipo);
    // Liberando memória alocada para o alien (ALIEN*)
    free(alien);
}

void destroi_grupo_aliens(GRUPO_ALIENS * grupo, int n)
{

    // Liberando a memória alocada por cada alien do esquadrão(ALIEN*)
    for(int i = 0; i < n; i++)
    {
        destroi_alien(grupo->esquadrao[i]);
    }
    // Liberando memória alocada para todo esquadrão (ALIEN**)
    free(grupo->esquadrao);
    // Liberando memória alocada para o tipo do esquadrão (char*)
    free(grupo->tipo);
    // Liberando memória alocada para o grupo de aliens (GRUPO_ALIENS*)
    free(grupo);
}

void destroi_esquadrao(GRUPO_ALIENS ** aliens, int tamanho_esq, int n)
{
    for(int i = 0; i < n; i++)
    {
        destroi_grupo_aliens(aliens[i], tamanho_esq);
    }
    free(aliens);
}

void draw_scenario()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, SCREEN_H - GRASS_H, SCREEN_W, SCREEN_H, al_map_rgb(0, 240, 0));
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


    NAVE nave;
    initNave(&nave);

    int tamanho_esq = 6;
    int num_esq = 4;
    GRUPO_ALIENS ** aliens = cria_esquadroes(tamanho_esq, num_esq);

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

            for(int i = 0; i < num_esq; i++)
            {
                update_grupo_aliens(aliens[i]);
            }

            update_tiro(&nave.municao, nave.x);
            draw_nave(nave);

            for(int i = 0; i < num_esq; i++)
            {
                imprime_grupo_aliens(aliens[i]->esquadrao, tamanho_esq);
            }

            draw_tiro(nave.municao);
            colisao_tiro(&nave.municao, nave.x,aliens, tamanho_esq, num_esq);
            // playing = !colisao_alien_solo(&alien);

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
                case ALLEGRO_KEY_SPACE:
                    nave.municao.estado = 1;
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

    destroi_esquadrao(aliens, tamanho_esq, num_esq);
    al_destroy_display(display);
    al_destroy_event_queue(fila_de_eventos);
 
    al_rest(1);
	return 0;
}