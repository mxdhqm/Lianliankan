#include "icons.h"
#include <SDL/SDL_image.h>

struct icons *icons_init(int size){
	struct icons *icons;

	icons = calloc(1, sizeof(*icons));
	if(!icons){
		return NULL;
	}
	icons->size = size;

	icons->images = calloc(size, sizeof(*icons->images));
	if(!icons->images){
		free(icons);
		return NULL;
	}

	return icons;
}

void icons_free(struct icons *icons){
	if(icons->images){
		free(icons->images);
	}
	free(icons);
}

int icons_load_image(struct icons *icons, char *directory){
	SDL_Surface *image;
	char file_name[256];
	int i;
	
	sprintf(file_name, "%s/start.png", directory);
	icons->startbg = IMG_Load(file_name);
	if (icons->startbg == NULL) {
		icons_free_image(icons);
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return -1;
	}
	
	sprintf(file_name, "%s/success.png", directory);
	icons->endbg = IMG_Load(file_name);
	if (icons->startbg == NULL) {
		icons_free_image(icons);
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return -1;
	}
	
	sprintf(file_name, "%s/background.png", directory);
	icons->background = IMG_Load(file_name);
	if (icons->background == NULL) {
		icons_free_image(icons);
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return -1;
	}
	sprintf(file_name, "%s/frame_normal.png", directory);
	icons->frame_normal = IMG_Load(file_name);
	if (icons->frame_normal == NULL) {
		icons_free_image(icons);
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return -1;
	}
	sprintf(file_name, "%s/frame_select.png", directory);
	icons->frame_select = IMG_Load(file_name);
	if (icons->frame_select == NULL) {
		icons_free_image(icons);
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return -1;
	}
	sprintf(file_name, "%s/boom.png", directory);
	icons->boom = IMG_Load(file_name);
	if (icons->boom == NULL) {
		icons_free_image(icons);
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return -1;
	}
	sprintf(file_name, "%s/line_vertical.png", directory);
	icons->line_vertical = IMG_Load(file_name);
	if (icons->line_vertical == NULL) {
		icons_free_image(icons);
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return -1;
	}
	sprintf(file_name, "%s/line_horizontal.png", directory);
	icons->line_horizontal = IMG_Load(file_name);
	if (icons->line_horizontal == NULL) {
		icons_free_image(icons);
		fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
		return -1;
	}

	/* 0 不使用 */
	for(i = 1; i <= icons->size; i++){
		sprintf(file_name, "%s/%d.png", directory, i);
		image = IMG_Load(file_name);
		if (image == NULL) {
			icons_free_image(icons);
			fprintf(stderr, "Couldn't load %s: %s\n", file_name, SDL_GetError());
			return -1;
		}

		icons->images[i] = image;
	}

	return 0;
}

void icons_free_image(struct icons *icons){
	int i;

	if(icons->background){
		SDL_FreeSurface(icons->background);
	}
	if(icons->frame_normal){
		SDL_FreeSurface(icons->frame_normal);
	}
	if(icons->frame_select){
		SDL_FreeSurface(icons->frame_select);
	}
	if(icons->boom){
		SDL_FreeSurface(icons->boom);
	}
	if(icons->line_vertical){
		SDL_FreeSurface(icons->line_vertical);
	}
	if(icons->line_horizontal){
		SDL_FreeSurface(icons->line_horizontal);
	}

	for(i = 0; i < icons->size; i++){
		if(icons->images[i]){
			SDL_FreeSurface(icons->images[i]);
		}
	}
}
