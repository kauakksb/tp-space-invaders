#include "libs_e_tipos.h"

// Armazena a matriz do desenho de uma figura
int ** guarda_matriz(int *linhas, int *colunas, char * nome_arquivo)
{
    int ** desenho;
    FILE * arq = fopen(nome_arquivo, "r");

    if(arq == NULL)
    {
        printf("Failed to open file\n");
        return 0;
    }

    fscanf(arq, "%d", linhas);
    fscanf(arq, "%d", colunas);

    desenho = (int**) malloc(*linhas * sizeof(int*));
    for(int i = 0; i < *linhas; i++)
    {
        desenho[i] = (int*) malloc(*colunas * sizeof(int));
    }

    for(int i = 0; i < *linhas; i++)
    {
        for(int j = 0; j < *colunas; j++)
        {
            fscanf(arq, "%d", &desenho[i][j]);
        }
    }

    fclose(arq);

    for(int i = 0; i < *linhas; i++)
    {
        for(int j = 0; j < *colunas; j++)
        {
            printf("%d ", desenho[i][j]);
        }
        printf("\n");
    }

    return desenho;
}

// Verifica se o tiro da nave colidiu com algum alien
int colisao_tiro(TIRO * municao, float novo_x,float novo_y, GRUPO_ALIENS ** aliens, int tamanho_esq, int n)
{
    if(municao->estado == 1)
    {
        for(int num_esq = 0; num_esq < n; num_esq++)
        {
            for(int alien_esquadrao = 0; alien_esquadrao < tamanho_esq; alien_esquadrao++)
            {
                if(aliens[num_esq]->esquadrao[alien_esquadrao]->estado == 1)
                {
                    int x1 = aliens[num_esq]->esquadrao[alien_esquadrao]->x;
                    int y1 = aliens[num_esq]->esquadrao[alien_esquadrao]->y;
                    int x2 = aliens[num_esq]->esquadrao[alien_esquadrao]->x + aliens[num_esq]->esquadrao[alien_esquadrao]->largura;
                    int y2 = aliens[num_esq]->esquadrao[alien_esquadrao]->y + aliens[num_esq]->esquadrao[alien_esquadrao]->altura;
                    for(int linha_tiro = 0; linha_tiro < municao->linhas; linha_tiro++)
                    {
                        for(int coluna_tiro = 0; coluna_tiro < municao->colunas; coluna_tiro++)
                        {
                            if (municao->x + (coluna_tiro*2) > x1 && municao->x + municao->x +(coluna_tiro*2) < x2)
                            {
                                if(municao->y + (linha_tiro*2) > y1 && municao->y + (linha_tiro*2) < y2)
                                {
                                    printf("\ncolidiu\n");
                                    municao->estado = 0;
                                    municao->y = novo_y;
                                    municao->x = novo_x;
                                    aliens[num_esq]->esquadrao[alien_esquadrao]->estado = 0;
                                    printf("\nestado alien colidido: %d\n", aliens[num_esq]->esquadrao[alien_esquadrao]->estado);
                                    return 1;
                                }
                            }
                            else if(municao->x + (coluna_tiro*2)+2 > x1 && municao->x + (coluna_tiro*2)+2 < x2)
                            {
                                if(municao->y + (linha_tiro*2)+2 > y1 && municao->y + (linha_tiro*2)+2 < y2)
                                {
                                    printf("colidiu");
                                    municao->estado = 0;
                                    municao->y = novo_y;
                                    municao->x = novo_x;
                                    aliens[num_esq]->esquadrao[alien_esquadrao]->estado = 0;
                                    printf("\nestado alien colidido: %d\n", aliens[num_esq]->esquadrao[alien_esquadrao]->estado);
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

// Verifica se algum alien está muito próximo da nave
int alien_proximo_nave(NAVE * nave, GRUPO_ALIENS ** aliens, int tamanho_esq, int num_esq)
{
    for(int i = 0; i < num_esq; i++)
    {
        if(aliens[i]->estado)
        {
            for(int j = 0; j < tamanho_esq; j++)
            {
                if(aliens[i]->esquadrao[j]->estado)
                {
                    
                    float alien_x1 = aliens[i]->esquadrao[j]->x;
                    float alien_x2 = aliens[i]->esquadrao[j]->x + aliens[i]->esquadrao[j]->largura;
                    float alien_y = aliens[i]->esquadrao[j]->y + aliens[i]->esquadrao[j]->altura;

                    float nave_x = nave->x + (nave->largura/2);
                    float nave_y = nave->y;

                    double dist1 = sqrt((alien_x1 - nave_x)*(alien_x1 - nave_x) + (alien_y - nave_y)*(alien_y - nave_y));
                    double dist2 = sqrt((alien_x2 - nave_x)*(alien_x2 - nave_x) + (alien_y - nave_y)*(alien_y - nave_y));

                    if(dist1 < 20 || dist2 < 20)return 1;
                    
                }
            }
        }
    }
    return 0;
}

// Verifica se todos os aliens foram atingidos
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

// Verifica se o score foi superior ao recorde e caso seja armazena num arquivo
int guarda_recorde(int score, int recorde)
{
    if(score > recorde)
    {
        FILE * arq = fopen("recorde.txt", "w");

        fprintf(arq, "%d", score);

        fclose(arq);
        return 1;
    }
    return 0;
}
