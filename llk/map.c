#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "map.h"

struct map *map_init(int width, int height, int cell_types){
	struct map *map;

	map = calloc(1, sizeof(*map));
	if(!map){
		return NULL;
	}
	map->width = width;
	map->height = height;
	map->cell_types = cell_types;
	map->cell = calloc(map->width * map->height, sizeof(*map->cell));
	if(!map->cell){
		free(map);
		return NULL;
	}

	return map;
}

void map_free(struct map *map){
	if(map->cell){
		free(map->cell);
	}
	free(map);
}

int map_generate(struct map *map){
	int i, val, istrue;
	/*static int seed = 0;
	istrue = 1;
	if(seed == 0){
		seed = time(NULL);
	}else{
		seed++;
	}
	*/
	/*
	TODO: 地图生成算法:
	1. 随机选择一个位置, 如果被占用, 向前查找到第一个未占用的的位置, 还未找到则向后找.
	2. 同样的方法选择第二个空位, 但先向后查找再向前查找.
	3. 设置两个位置为相同的地形.
	*/
	
	
	//srand(seed);
	
	int a,b,c,d,e;
	a = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	
	while (istrue)
	{
		for(i = 0; i < map->width * map->height; i++){
			val = rand() % map->cell_types;
			map->cell[i] = val;
			//printf("%d\n",val);
			if(val == 0)
				a ++;
			else if(val == 1)
				b ++;
			else if(val == 2)
				c ++;
			else if(val == 3)
				d ++;
			else if(val == 4)
				e ++;
		}
		//printf ("%d -- %d -- %d -- %d -- %d",a,b,c,d,e);
		if(a % 2 == 0 && b % 2 == 0 && c % 2 == 0 && d % 2 == 0 && e % 2 == 0)
			istrue = 0;
		else
		{
			a = 0;
			b = 0;
			c = 0;
			d = 0;
			e = 0;
		}	
	}

	return 0;
}

int map_point_is_valid(struct map *map, struct point *p){
	return p->x >= 0 && p->x <= map->width
		&& p->y >= 0 && p->y <= map->height;
}

/*
判断x坐标相同或者y坐标相同的两点之间是否直接连通的.
不连通返回0, 连通返回其它.
*/
static int map_connected_2(struct map *map, struct point *p1, struct point *p2){
	int i, val;
	int min, max;

	if(p1->x == p2->x){
		min = p1->y < p2->y? p1->y : p2->y;
		max = p1->y > p2->y? p1->y : p2->y;
		for(i = min + 1; i < max; i++){
			val = map_get_cell_value(map, p1->x, i);
			if(val > 0){
				return 0;
			}
		}
		return 1;
	}

	if(p1->y == p2->y){
		min = p1->x < p2->x? p1->x : p2->x;
		max = p1->x > p2->x? p1->x : p2->x;
		for(i = min + 1; i < max; i++){
			val = map_get_cell_value(map, i, p1->y);
			if(val > 0){
				return 0;
			}
		}
		return 1;
	}

	return 0;
}

/*
判断p1, p2是否通过line连通.
*/
static inline int map_connected(struct map *map, struct point *p1, struct point *p2,
		struct line *line){
	return (
		map_connected_2(map, p1, &line->p1)
		&& map_connected_2(map, &line->p1, &line->p2)
		&& map_connected_2(map, &line->p2, p2)
		&& (point_equal(p1, &line->p1) || map_get_cell_value(map, line->p1.x, line->p1.y) <= 0)
		&& (point_equal(p2, &line->p2) || map_get_cell_value(map, line->p2.x, line->p2.y) <= 0)
		);
};

int map_find_path(struct map *map, struct point *p1, struct point *p2,
		struct line *line){
	struct line lv1, lv2;
	struct line lh1, lh2;
	int val1, val2;
	int finish_count = 0;

	if(point_equal(p1, p2)){
		return -1;
	}

	val1 = map_get_cell_value(map, p1->x, p1->y);
	val2 = map_get_cell_value(map, p2->x, p2->y);
	if(val1 != val2 || val1 <= 0 || val2 <= 0){
		return -1;
	}

	/*
	lh1 是在与横坐标轴平行的线段上的两点连线, 向上(y--)运动
	lh2 是在与横坐标轴平行的线段上的两点连线, 向下(y++)运动
	lv1 是在与纵坐标轴平行的线段上的两点连线, 向左(x--)运动
	lv2 是在与纵坐标轴平行的线段上的两点连线, 向右(x++)运动
	*/

	lh1.p1.y = lh1.p2.y = max(p1->y, p2->y);
	lh1.p1.x = p1->x;
	lh1.p2.x = p2->x;

	lh2.p1.y = lh2.p2.y = min(p1->y, p2->y);
	lh2.p1.x = p1->x;
	lh2.p2.x = p2->x;

	lv1.p1.x = lv1.p2.x = max(p1->x, p2->x);
	lv1.p1.y = p1->y;
	lv1.p2.y = p2->y;

	lv2.p1.x = lv2.p2.x = min(p1->x, p2->x);
	lv2.p1.y = p1->y;
	lv2.p2.y = p2->y;

	while(1){
		if(lv1.p1.x >= 0){
			if(map_connected(map, p1, p2, &lv1)){
				*line = lv1;
				return 0;
			}
			lv1.p1.x--;
			lv1.p2.x--;
		}else{
			finish_count++;
		}
		if(lv2.p1.x < map->height){
			if(map_connected(map, p1, p2, &lv2)){
				*line = lv2;
				return 0;
			}
			lv2.p1.x++;
			lv2.p2.x++;
		}else{
			finish_count++;
		}
		if(lh1.p1.y >= 0){
			if(map_connected(map, p1, p2, &lh1)){
				*line = lh1;
				return 0;
			}
			lh1.p1.y--;
			lh1.p2.y--;
		}else{
			finish_count++;
		}
		if(lh2.p1.y < map->width){
			if(map_connected(map, p1, p2, &lh2)){
				*line = lh2;
				return 0;
			}
			lh2.p1.y++;
			lh2.p2.y++;
		}else{
			finish_count++;
		}

		/* 4条线的查找都已经结束 */
		if(finish_count == 4){
			return -1;
		}else{
			finish_count = 0;
		}
	}

	return -1;
}

/*
画两点之间的直连线.
*/
static void map_draw_line(struct map *map, struct point *p1, struct point *p2){
	int i, min, max;

	if(p1->x == p2->x){
		min = p1->y < p2->y? p1->y : p2->y;
		max = p1->y > p2->y? p1->y : p2->y;
		for(i = min; i <= max; i++){
			/* 用 -1 表示线条 */
			map_set_cell_value(map, p1->x, i, -1);
		}
		return;
	}

	if(p1->y == p2->y){
		min = p1->x < p2->x? p1->x : p2->x;
		max = p1->x > p2->x? p1->x : p2->x;
		for(i = min; i <= max; i++){
			map_set_cell_value(map, i, p1->y, -1);
		}
		return;
	}
}

void map_draw_path(struct map *map, struct point *p1, struct point *p2,
		struct line *line){
	map_draw_line(map, p1, &line->p1);
	map_draw_line(map, &line->p1, &line->p2);
	map_draw_line(map, &line->p2, p2);
	/* -2 表示两点 */
	map->cell[p1->y * map->width + p1->x] = -2;
	map->cell[p2->y * map->width + p2->x] = -2;
}

void map_clear_path(struct map *map){
	int i;

	for(i = 0; i < map->width * map->height; i++){
		if(map->cell[i] < 0){
			map->cell[i] = 0;
		}
	}
}

void map_show(struct map *map){
	int i, j, val;

	printf("-+---- x\n");
	printf(" |\n |\n y\n\n");

	printf("   ");
	for(i = 0; i < map->width; i++){
		printf(" %-2d", i);
	}
	printf("   \n");
	printf("  +");
	for(i = 0; i < map->width; i++){
		printf("---");
	}
	printf("+\n");

	for(i = 0; i < map->height; i++){
		printf("%-2d|", i);
		for(j = 0; j < map->width; j++){
			val = map_get_cell_value(map, j, i);
			if(val == -2){
				printf(" @ ");
			}else if(val == -1){
				printf(" * ");
			}else if(val == 0){
				printf("   ");
			}else{
				printf(" %-2d", val);
			}
		}
		printf("|%2d", i);
		printf("\n");
	}

	printf("  +");
	for(i = 0; i < map->width; i++){
		printf("---");
	}
	printf("+\n");
	printf("   ");
	for(i = 0; i < map->width; i++){
		printf(" %-2d", i);
	}
	printf("\n");
}

int map_end(struct map *map)
{
	int i, j, val;
	for(i = 0; i < map->height; i++){
		for(j = 0; j < map->width; j++){	
			val = map_get_cell_value(map, j, i);
			if(val > 0)
			{
				return 1;
			}
		}
	}
	return 0;
}
