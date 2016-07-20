#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "map.h"
#include "icons.h"

#define GRID_ROWS	10
#define GRID_COLS	13

#define CELL_TYPES	5
#define CELL_SIZE	48

int flag;				//flag 0 未开始， 1 已开始

struct world{
	struct map *map;

	SDL_Surface *screen;
	struct icons *icons;

	struct point click;
};

void init(struct world *world){
	world->click.x = world->click.y = -1;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Could not initializing SDL: %s.\n",SDL_GetError());
		exit(-1);
	}
	atexit(SDL_Quit);

	//world->screen = SDL_SetVideoMode(GRID_COLS * CELL_SIZE, GRID_ROWS * CELL_SIZE,
		//32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	world->screen = SDL_SetVideoMode(640, 480,
		16, SDL_HWSURFACE);
	if(world->screen == NULL){
		fprintf(stderr, "Couldn't set 800x600x32 video mode: %s\n", SDL_GetError());
		exit(1);
	}

	world->icons = icons_init(TERRAIN_TYPE_COUNT);
	if(!world->icons){
		fprintf(stderr, "icons_init error.\n");
		exit(0);
	}
	if(icons_load_image(world->icons, "icon1") == -1){
		fprintf(stderr, "icons_load_image error.\n");
		exit(0);
	}

	world->map = map_init(GRID_COLS, GRID_ROWS, CELL_TYPES);
	if(!world->map){
		fprintf(stderr, "map_init error!");
		return;
	}
	map_generate(world->map);


	//map_show(world->map);
}


void refresh(struct world *world){
	SDL_Surface *screen;
	SDL_Surface *image;
	SDL_Rect icon_r, cell_r;
	int val;
	struct point p;

	screen = world->screen;

	//SDL_FillRect(screen, NULL, 0);
	for(p.x = 0; p.x < world->map->width; p.x++){
		for(p.y = 0; p.y < world->map->height; p.y++){
			cell_r.x = p.x * CELL_SIZE;
			cell_r.y = p.y * CELL_SIZE;
			icon_r.x = p.x * CELL_SIZE + 1;
			icon_r.y = p.y * CELL_SIZE + 1;

			/* background */
			image = world->icons->background;
			if(SDL_BlitSurface(image, NULL, screen, &cell_r) < 0){
				fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
			}

			/* cell */
			val = map_get_cell_value(world->map, p.x, p.y);
			if(val == -2){
				image = world->icons->boom;
			}else if(val == -1){
				image = world->icons->line_vertical;
			}else if(val > 0){
				image = world->icons->images[val];
			}else{
				image = NULL;
			}
			if(image){
				if(SDL_BlitSurface(image, NULL, screen, &icon_r) < 0){
					fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
				}
			}

			/* frame */
			if(point_equal(&world->click, &p)){
				image = world->icons->frame_select;
			}else{
				image = world->icons->frame_normal;
			}
			if(SDL_BlitSurface(image, NULL, screen, &cell_r) < 0){
				fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
			}
			//else
		}
	}
	SDL_Flip(screen);
}

void gameStart(struct world *world)
{
	flag = 0;
	SDL_Surface *screen;
	screen = world->screen;
	SDL_Rect offset;
    offset.x = 0;
    offset.y = 0;
	SDL_BlitSurface( world->icons->startbg, NULL, screen, &offset );
	SDL_Flip(screen);
}

void gameEnd(struct world *world)
{
	//flag = 0;
	SDL_Surface *screen;
	screen = world->screen;
	SDL_Rect offset;
    offset.x = 0;
    offset.y = 0;
	SDL_BlitSurface( world->icons->endbg, NULL, screen, &offset );
	SDL_Flip(screen);	
}

#define TICK_INTERVAL	15

int main(int argc, char **argv){
	SDL_Event event;
	struct line line;
	struct point point;
	struct world world;
	struct map *map;
	int ret;
	
	init(&world);
	//refresh(&world);
	gameStart(&world);
	
	map = world.map;
	while(SDL_WaitEvent(&event) >= 0){
		switch(event.type){
			case SDL_MOUSEBUTTONDOWN:
				if(flag == 0)
				{
					if(event.motion.x >= 140 && event.motion.x <= 504 && event.motion.y >= 302 && event.motion.y <= 384)
					{
						flag = 1;
						SDL_FillRect(world.screen, NULL, 0);
						refresh(&world);
					}
				}
				else
				{
					map_clear_path(map);

					point.x = event.motion.x / CELL_SIZE;
					point.y = event.motion.y / CELL_SIZE;
					//printf("click on pixel(%d, %d), cell(%d, %d)\n", event.motion.x, event.motion.y, point.x, point.y);

					if(point_equal(&point, &world.click)){
						/* 点击同一格子两次, 则取消选择 */
						world.click.x = world.click.y = -1;
					}else{
						if(world.click.x == world.click.y == -1){
							world.click = point;
						}else{
							ret = map_find_path(map, &world.click, &point, &line);
							if(ret == -1){
								world.click = point;
							}else{
								map_draw_path(map, &world.click, &point, &line);
								//printf("path: (%d,%d) - (%d,%d) - (%d,%d) - (%d,%d)\n",
									//world.click.x, world.click.y, line.p1.x, line.p1.y,
									//line.p2.x, line.p2.y, point.x, point.y);
								//map_show(world.map);
								world.click.x = world.click.y = -1;
								
								}
							}
						}
						if(map_end(world.map) == 0)
						{
							gameEnd(&world);
						}
						else
						{
							refresh(&world);
						}
					}
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
					case SDLK_q:
						exit(0);
						break;
					case SDLK_r:
						map_generate(map);
						SDL_FillRect(world.screen, NULL, 0);
						refresh(&world);
						//map_show(world.map);
						break;
					case SDLK_p:
						map_show(world.map);
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT:
				exit(0);
				break;
			default:
				break;
		}
	}

	return 0;
}
