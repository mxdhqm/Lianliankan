CC = gcc
CFLAGS = -I. -Wall -g
RELEASE_CFLAGS = -I.
CLIBS = -lSDL -lSDL_image

all: gui cli

cli: map.c llk_cli.c
	${CC} ${CFLAGS} -o cli map.c llk_cli.c

cli_release:
	${CC} ${RELEASE_CFLAGS} -o cli map.c llk_cli.c

gui: map.c main.c
	${CC} ${CFLAGS} -o gui main.c map.c icons.c ${CLIBS}

clean:
	rm -f cli gui
	
