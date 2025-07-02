#include "libs_e_tipos.h"

// Instancia uma struct nave
NAVE * initNave(char * tipo);

// Desenha a nave do jogador no cenário
void draw_nave(NAVE *nave);

// Atualiza a posição da nave de acordo com parâmetros de controle da nave
void update_nave(NAVE *nave);

// Reseta as variáveis de nave para uma nova rodada
void reset_nave(NAVE * nave);

// Desenha o tiro do jogador no cenário
void draw_tiro(TIRO municao);

// Atualiza a posição do tiro e retorna para nave caso não acerte nenhum alvo
void update_tiro(TIRO * municao, float novo_x);

// Libera a memória alocada para a struct nave e o seu design
void destroi_nave(NAVE * nave);