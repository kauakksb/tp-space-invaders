#include "libs_e_tipos.h"
#include "nave.h"


// Cria uma instância de jogador
JOGADOR * initJogador(char * nome,int id, int vidas, char * tipo_nave)
{
    JOGADOR * jogador = (JOGADOR*) malloc(sizeof(JOGADOR));

    jogador->nome = (char*) malloc(strlen(nome) * sizeof(char));
    strcpy(jogador->nome, nome);

    jogador->id = id;
    jogador->vidas = vidas;
    jogador->contador_pw = 0;
    jogador->num_tiros_pw = 10;
    jogador->score = 0;

    jogador->nave = initNave(tipo_nave);

    return jogador;
}

// Libera a memória alocada para uma struct de jogador 
void destroi_jogador(JOGADOR * jogador)
{
    destroi_nave(jogador->nave);
    free(jogador->nome);
    free(jogador);
}