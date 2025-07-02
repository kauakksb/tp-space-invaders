#include "libs_e_tipos.h"

// Cria uma instância de jogador
JOGADOR * initJogador(char * nome,int id, int vidas, char * tipo_nave);

// Libera a memória alocada para uma struct de jogador 
void destroi_jogador(JOGADOR * jogador);