#ifndef ICONS_H
#define ICONS_H

#include <SDL/SDL.h>
/* TODO: 重命名为 resource? */
struct icons{
	SDL_Surface *background;
	SDL_Surface *frame_normal;
	SDL_Surface *frame_select;

	SDL_Surface *boom;
	SDL_Surface *line_vertical;
	SDL_Surface *line_horizontal;
	
	SDL_Surface *startbg;
	SDL_Surface *endbg;

	int size;
	SDL_Surface **images;
};

struct icons *icons_init(int size);
void icons_free(struct icons *icons);

int icons_load_image(struct icons *icons, char *directory);
void icons_free_image(struct icons *icons);

#endif
