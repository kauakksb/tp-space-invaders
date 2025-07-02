
# Arquivo executável
TARGET = tp-pds-1.exe

# Arquivos fonte
SRC = tp-pds-1.c modulos/alien.c modulos/nave.c modulos/util.c modulos/jogador.c modulos/designs.c

# Arquivos fonte compilados
OBJ = $(SRC:.c=.o)

# Compilador
CC = gcc

# Diretórios
ALLEGRO_DIR = c:\allegro-5.2.10.0\allegro


CFLAGS = -I$(ALLEGRO_DIR)\include\ -Wall -mconsole
LDFLAGS = -L$(ALLEGRO_DIR)\lib 

LIBS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives 

# Define o que será criado quando o Makefile é executado sem argumentos
all: $(TARGET)

# Compilação do arquivo executável/ principal
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $@ $(LIBS)

# Compilação dos arquivos .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	del /Q $(TARGET) $(OBJ)