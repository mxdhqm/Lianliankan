#ifndef MAP_H
#define MAP_H

#ifndef min
#	define min(a, b)	(((a)<(b))?(a):(b))
#endif
#ifndef max
#	define max(a, b)	(((a)>(b))?(a):(b))
#endif

#define TERRAIN_TYPE_COUNT		4 /* 地形的种类数量 */

struct map{
	int width;
	int height;

	int cell_types;
	int *cell;
};

struct point{
	int x;
	int y;
};

struct line{
	struct point p1;
	struct point p2;
};

struct map *map_init(int width, int height, int terrain_types);
void map_free(struct map *map);
/*
生成地图.
*/
int map_generate(struct map *map);
int map_point_is_valid(struct map *map, struct point *p);

/*
查找p1, p2两点之间的连通路径, 保存在 line 中.
返回-1表示没有找到连通的路径.
*/
int map_find_path(struct map *map, struct point *p1, struct point *p2,
		struct line *line);
/*
画两点之间的连通线.
*/
void map_draw_path(struct map *map, struct point *p1, struct point *p2,
		struct line *line);
/*
清除屏幕上的连通线.
*/
void map_clear_path(struct map *map);
/*
显示出整个地图.
*/
void map_show(struct map *map);
/*
游戏是否结束
*/
int map_end(struct map *map);

static inline int point_equal(struct point *p1, struct point *p2){
	return p1->x == p2->x && p1->y == p2->y;
}

static inline int map_get_cell_value(struct map *map, int x, int y){
	return map->cell[y * map->width + x];
}

static inline void map_set_cell_value(struct map *map, int x, int y, int value){
	map->cell[y * map->width + x] = value;
}

#endif
