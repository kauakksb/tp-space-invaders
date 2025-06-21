#include <stdio.h>
#include <stdlib.h>

// Arquivo para teste de funções a serem implementadas no projeto

int main()
{
    FILE * arq = fopen("crab.txt", "r");

    int **desenho;
    int linhas, colunas;

    fscanf(arq, "%d", &linhas);
    fscanf(arq, "%d", &colunas);

    printf("linhas: %d\n", linhas);
    printf("colunas: %d\n", colunas);

    desenho = (int**) malloc(linhas * sizeof(int*));
    printf("Alocou espaco (int**)");

    for(int i = 0; i < linhas; i++)
    {
        desenho[i] = (int*) malloc(colunas * sizeof(int));
        printf("alocou espaco: %d\n", i);
    }
    printf("alocou todos os espacos\n");

    for(int i = 0; i < linhas; i++)
    {
        for(int j = 0; j < colunas; j++)
        {
            fscanf(arq, "%d", &desenho[i][j]);
            printf("escaneou [%d][%d]\n", i, j);
        }
    }
    fclose(arq);

    for(int i = 0; i < linhas; i++)
    {
        for(int j = 0; j < colunas; j++)
        {
            printf("%d ", desenho[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < colunas; i++)
    {
        free(desenho[i]);
    }
    free(desenho);

    printf("concluído\n");
    return 0;
}