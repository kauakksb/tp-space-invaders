#include "libs_e_tipos.h"
#include "util.h"

#define SCREEN_W 960 // Largura da tela
#define SCREEN_H 540 // Altura da tela=
#define GRASS_H 60

// Instancia uma struct de ALIEN e retorna seu endereço de memória (o seu ponteiro)
ALIEN *initAlien(char *tipo, int id, int grupo, float espacamento_x, float espacamento_y)
{
    // Criando ponteiro de ALIEN
    ALIEN *alien;

    // Alocando espaço para a struct ALIEN
    alien = (ALIEN*) malloc(sizeof(ALIEN));

    alien->estado = 1;

    // Armazenando id e grupo do alien
    alien->id = id;
    alien->grupo = grupo;

    // Alocando espaço para o tipo do alien e guardando o tipo
    alien->tipo = (char*) malloc(strlen(tipo) * sizeof(char));
    strcpy(alien->tipo, tipo);

    // Verificando qual o tipo do alien
    if(!strcmp(alien->tipo, "carangueijo"))
    {
        alien->desenho = guarda_matriz(&alien->linhas, &alien->colunas, "designs/crab.txt");
        // arq = fopen("designs/crab.txt", "r");
    }
    else if(!strcmp(alien->tipo, "polvo"))
    {
        alien->desenho = guarda_matriz(&alien->linhas, &alien->colunas, "designs/octopus.txt");
        // arq = fopen("designs/octopus.txt", "r");
    }
    else if(!strcmp(alien->tipo, "molusco"))
    {
        alien->desenho = guarda_matriz(&alien->linhas, &alien->colunas, "designs/squid.txt");
        // arq = fopen("designs/squid.txt", "r");
    }

    // Guarda as medidas de altura e largura baseada no desenho
    alien->altura = 5 * alien->linhas;
    alien->largura = 5 * alien->colunas; 

    // Armazena as posições iniciais do alien baseado em no grupo ao qual o alien pertence e o seu id no grupo
    alien->x = espacamento_x + (alien->largura + espacamento_x) * alien->id;
    alien->y = 20 + espacamento_y + (alien->altura + espacamento_y) * alien->grupo; 

    // Determina as velocidades de movitação do alien na vertical e na horizontal
    alien->x_vel = 1;
    alien->y_vel = alien->altura;
    
    // Armazena uma cor para o alien
    alien->cor = al_map_rgb(rand()%255, rand()%255, rand()%255);

    // Retorna o endereço de memória da struct
    return alien;
}

// Realiza a instância de um grupo de aliens e os organiza em um esquadrão
GRUPO_ALIENS *init_grupo_aliens(int n, int id_grupo, char * tipo, float espacamento_x, float espacamento_y)
{
    GRUPO_ALIENS *grupo_aliens = (GRUPO_ALIENS*) malloc(sizeof(GRUPO_ALIENS));
    
    grupo_aliens->esquadrao = (ALIEN**) malloc(n * sizeof(ALIEN*));
    grupo_aliens->id_grupo = id_grupo;
    grupo_aliens->estado = 1;
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

    printf("Largura esquadrao: %f\n", grupo_aliens->largura);
    printf("Altura esquadrao: %f\n", grupo_aliens->altura);

    grupo_aliens->altura = grupo_aliens->esquadrao[0]->altura;

    return grupo_aliens;
}

// Realiza a criação de vários esquadrões e os organiza em um batalhão
GRUPO_ALIENS ** cria_esquadroes(int tamanho_esq, int n, float espacamento_x, float espacamento_y)
{
    GRUPO_ALIENS** aliens = (GRUPO_ALIENS **) malloc(n * sizeof(GRUPO_ALIENS*));

    for(int i = 0; i < n; i++)
    {
        aliens[i] = init_grupo_aliens(tamanho_esq, i, "polvo", espacamento_x, espacamento_y);
    }

    return aliens;
}

void define_aliens_used(GRUPO_ALIENS ** aliens, int tamanho_esq, int num_esq, int novo_tamanho, int novo_num)
{
    for(int i = 0; i < num_esq; i++)
    {
        for(int j = 0; j < tamanho_esq; j++)
        {
            if(j >= novo_tamanho)
            {
                aliens[i]->esquadrao[j]->estado = 0;
            }
            if(i >= novo_num)
            {
                aliens[i]->esquadrao[j]->estado = 0;
                aliens[i]->estado = 0;
            }
        }
    }

    for(int i = 0; i < novo_num; i++)
    {
        aliens[i]->largura = 0;
        int j = 0;
        while(1)
        {
            aliens[i]->largura += aliens[i]->esquadrao[j]->largura;
            if(j == novo_tamanho - 1)break;
            aliens[i]->largura += aliens[i]->espacamento_x;
            j++;
        }
    }
    printf("reconfiguracao concluida!");
}

// Desenha a figura do alien no display
void draw_alien(ALIEN *alien)
{
    if(alien->estado)
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
                else if(alien->desenho[i][j] == 2)
                {
                    al_draw_filled_rectangle(alien->x + (j*5), alien->y + (i*5), 
                                alien->x + (j+1) * 5, alien->y + (i+1) * 5, 
                                al_map_rgb(0, 0, 255));
                }
            }
        }
    }
}

// Desenha um grupo de aliens(esquadrão)
void imprime_grupo_aliens(ALIEN** grupo, int n)
{
    for(int i = 0; i < n; i++)
    {
        draw_alien(grupo[i]);
    }
}

// Atualiza a posição do aliens para fins de movimentação
void update_alien(ALIEN *alien)
{
    if(alien->x + alien->largura + alien->x_vel > SCREEN_W || alien->x + alien->x_vel < 0)
    {
        alien->y += alien->altura;
        alien->x_vel *= -1;
    }
    alien->x += alien->x_vel;
}

// Atualiza a posição do esquadrão de forma que se movam em conjunto
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

    int contador = 0;
    for(int i = 0; i < grupo_aliens->num_aliens; i++)
    {
        if(grupo_aliens->esquadrao[i]->estado == 1)contador++;
    }

    if(contador == 0)grupo_aliens->estado = 0;

    for(int i = 0; i < grupo_aliens->num_aliens; i++)
    {
        update_alien(grupo_aliens->esquadrao[i]);
    }
    grupo_aliens->x += grupo_aliens->esquadrao[0]->x_vel;
    
}

// Verifica se um alien colidiu com o solo do cenário
int colisao_alien_solo(ALIEN *alien)
{
    if(alien->y + alien->altura > SCREEN_H - GRASS_H)return 1;
    return 0;
}

/* 
    Realiza a verificação se algum alien colidiu com o solo levando em conta fatores 
    como o estado do esquadrão e os estado do alien, isto é, se estão vivos ou não
*/
int verifica_colisao_esquadrao(GRUPO_ALIENS **aliens, int n)
{
    for(int i = 0; i < n; i++)
    {
        if(aliens[i]->estado)
        {
            for(int j = 0; j < aliens[i]->num_aliens; j++)
            {
                if(aliens[i]->esquadrao[j]->estado)
                {
                    if(colisao_alien_solo(aliens[i]->esquadrao[j]))return 1;
                }
            }
        }
    }
    return 0;
}

// Reseta grupo de aliens para nova rodada
void reset_alien(GRUPO_ALIENS ** aliens, int num_esq, int tamanho_esq, float espacamento_x, float espacamento_y)
{

    for(int i = 0; i < num_esq; i++)
    {
        for(int j = 0; j < tamanho_esq; j++)
        {
            aliens[i]->esquadrao[j]->x_vel = 1;
            aliens[i]->esquadrao[j]->estado = 1;
            aliens[i]->esquadrao[j]->x = espacamento_x + (aliens[i]->esquadrao[j]->largura + espacamento_x) * aliens[i]->esquadrao[j]->id;
            aliens[i]->esquadrao[j]->y = 40 + espacamento_y + (aliens[i]->esquadrao[j]->altura + espacamento_y) * aliens[i]->esquadrao[j]->grupo; 
        }
        aliens[i]->estado = 1;
        aliens[i]->x = aliens[i]->esquadrao[0]->x;
        aliens[i]->y = aliens[i]->esquadrao[0]->y;
    }



}


// Libera a memória alocada para a struct ALIEN passada no parâmetro
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

/* 
    Libera a memória alocada para a struct de cada alien do grupo e a 
    memória alocada para a struct do grupo todo
*/
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

// Libera a memória alocada para o conjunto de esquadrões
void destroi_esquadrao(GRUPO_ALIENS ** aliens, int tamanho_esq, int n)
{
    for(int i = 0; i < n; i++)
    {
        destroi_grupo_aliens(aliens[i], tamanho_esq);
    }
    free(aliens);
}
