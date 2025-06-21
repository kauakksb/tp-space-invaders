
TARGET = tp-pds-1.exe
SRC = tp-pds-1.c
OBJ = $(SRC:.c=.o)
CC = gcc

ALLEGRO_DIR = c:\allegro-5.2.10.0\allegro


CFLAGS = -I$(ALLEGRO_DIR)\include\ -Wall -mconsole
LDFLAGS = -L$(ALLEGRO_DIR)\lib 

LIBS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives 


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(TARGET) $(OBJ)