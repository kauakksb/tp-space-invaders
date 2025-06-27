#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Armazena a matriz do desenho de uma figura
int ** guarda_desenho(int *linhas, int *colunas, char * nome_arquivo)
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