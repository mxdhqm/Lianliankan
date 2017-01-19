#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"

#define CELL_TYPES	4

int main(int argc, char **argv){
	struct map *map;
	struct line line;
	struct point p1, p2;
	int width, height;
	int ret;

	if(argc == 2){
		width = height = atoi(argv[1]);
	}else{
		width = 5;
		height = 5;
	}

	map = map_init(width, height, CELL_TYPES);
	if(!map){
		fprintf(stderr, "map_init error!");
		return 0;
	}
	map_generate(map);

	while(1){
		map_show(map);
		map_clear_path(map);

		printf("input point(x1,y1 x2,y2): ");
		ret = scanf("%d,%d %d,%d", &p1.x, &p1.y, &p2.x, &p2.y);
		if(ret != 4){
			break;
		}
		if(!map_point_is_valid(map, &p1) || !map_point_is_valid(map, &p2)){
			printf("invalid point.\n");
		}

		printf("p1=%d, p2=%d\n",
			map_get_cell_value(map, p1.x, p1.y),
			map_get_cell_value(map, p2.x, p2.y));


		ret = map_find_path(map, &p1, &p2, &line);
		if(ret != -1){
			map_draw_path(map, &p1, &p2, &line);
			printf("path: (%d,%d) - (%d,%d) - (%d,%d) - (%d,%d)\n",
				p1.x, p1.y, line.p1.x, line.p1.y,
				line.p2.x, line.p2.y, p2.x, p2.y);
		}
	}

	printf("exit.");
	map_free(map);
	return 0;
}
