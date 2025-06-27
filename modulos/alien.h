#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>

/*  
    Struct que compõe um alien e suas propriedades, bem como 
    outros elementos para controle de cada um no jogo 
*/
typedef struct ALIEN
{
    char *tipo;
    int **desenho;
    int estado;
    int id, grupo;
    int linhas, colunas;
    float altura, largura;
    float x, y;
    float x_vel, y_vel;
    ALLEGRO_COLOR cor;
}ALIEN;

/*  
    Struct que contém um esquadrão/vetor de aliens e as propriedades desse 
    grupo, que são usadas para controlar aspectos como a movimentação do 
    grupo e os sistemas do jogo 
*/
typedef struct GRUPO_ALIENS
{
    ALIEN **esquadrao;
    char *tipo;
    int id_grupo;
    int estado;
    float x, y;
    float espacamento_x;
    float espacamento_y;
    float altura, largura;
    int num_aliens;
}GRUPO_ALIENS;

// Instancia uma struct de ALIEN e retorna seu endereço de memória (o seu ponteiro)
ALIEN *initAlien(char *tipo, int id, int grupo, float espacamento_x, float espacamento_y);

// Realiza a instância de um grupo de aliens e os organiza em um esquadrão
GRUPO_ALIENS *init_grupo_aliens(int n, int id_grupo, char * tipo, float espacamento_x, float espacamento_y);

// Realiza a criação de vários esquadrões e os organiza em um batalhão
GRUPO_ALIENS ** cria_esquadroes(int tamanho_esq, int n, float espacamento_x, float espacamento_y);

// Desenha a figura do alien no display
void draw_alien(ALIEN *alien);

// Desenha um grupo de aliens(esquadrão)
void imprime_grupo_aliens(ALIEN** grupo, int n);

// Atualiza a posição do aliens para fins de movimentação
void update_alien(ALIEN *alien);

// Atualiza a posição do esquadrão de forma que se movam em conjunto
void update_grupo_aliens(GRUPO_ALIENS * grupo_aliens);

// Verifica se um alien colidiu com o solo do cenário
int colisao_alien_solo(ALIEN *alien);

/* 
    Realiza a verificação se algum alien colidiu com o solo levando em conta fatores 
    como o estado do esquadrão e os estado do alien, isto é, se estão vivos ou não
*/
int verifica_colisao_esquadrao(GRUPO_ALIENS **aliens, int n);

// Libera a memória alocada para a struct ALIEN passada no parâmetro
void destroi_alien(ALIEN * alien);

/* 
    Libera a memória alocada para a struct de cada alien do grupo e a 
    memória alocada para a struct do grupo todo
*/
void destroi_grupo_aliens(GRUPO_ALIENS * grupo, int n);

// Libera a memória alocada para o conjunto de esquadrões
void destroi_esquadrao(GRUPO_ALIENS ** aliens, int tamanho_esq, int n);