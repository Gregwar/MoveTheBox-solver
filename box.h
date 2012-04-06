#ifndef BOX_H
#define BOX_H

struct box_map;

struct box_map *box_load(char *filename);

char box_solve(struct box_map *map, int tries);

#endif
