#include <stdio.h>
#include <stdlib.h>
#include "box.h"

#define BOX_MAP_WIDTH  7
#define BOX_MAP_HEIGHT 10

struct box_map {
    char map[BOX_MAP_WIDTH][BOX_MAP_HEIGHT];
};

struct box_map *box_map_create() {
    int line, col;
    struct box_map *map;

    map = malloc(sizeof(struct box_map));

    for (col=0; col<BOX_MAP_WIDTH; col++) {
        for (line=0; line<BOX_MAP_HEIGHT; line++) {
            map->map[col][line] = ' ';
        }
    }

    return map;
}

void box_print(struct box_map *map) {
    int line, col;

    for (line=0; line<BOX_MAP_HEIGHT; line++) {
        for (col=0; col<BOX_MAP_WIDTH; col++) {
            printf("%c", map->map[col][line]);
        }
        printf("\n");
    }
}

int box_gravity(struct box_map *map) {
    int line, col, bottom, count = 0;
    char c;

    for (col=0; col<BOX_MAP_WIDTH; col++) {
        bottom = BOX_MAP_HEIGHT-1;

        for (line=BOX_MAP_HEIGHT-1; line>=0; line--) {
            c = map->map[col][line];

            if (c != ' ') {
                count++;
                map->map[col][bottom] = c;

                if (bottom != line) {
                    map->map[col][line] = ' ';
                }

                bottom--;
            }
        }
    }

    return count;
}

char box_destroy(struct box_map *map) {
    int line, col, number=1, back;
    char chain, cur, changed = 0;
    struct box_map *destroy_map = box_map_create();

    for (line=0; line<BOX_MAP_HEIGHT; line++) {
        chain = 0;
        for (col=0; col<BOX_MAP_WIDTH; col++) {
            cur = map->map[col][line];

            if (cur != chain || col == BOX_MAP_WIDTH-1 || cur == ' ') {
                if (cur == chain && cur != ' ') {
                    number++;
                    col++;
                }
                if (number >= 3) {
                    changed = 1;
                    for (back=1; back<=number; back++) {
                        destroy_map->map[col-back][line] = 'D';
                    }
                }
                number = 1;
            } else {
                number++;
            }

            chain = cur;
        }
    }

    for (col=0; col<BOX_MAP_WIDTH; col++) {
        chain = 0;
        for (line=0; line<BOX_MAP_HEIGHT; line++) {
            cur = map->map[col][line];

            if (cur != chain || col==BOX_MAP_WIDTH-1 || cur == ' ') {
                if (cur == chain && cur != ' ') {
                    number++;
                    line++;
                }
                if (number >= 3) {
                    changed = 1;
                    for (back=1; back<=number; back++) {
                        destroy_map->map[col][line-back] = 'D';
                    }
                }
                number = 1;
            } else {
                number++;
            }

            chain = cur;
        }
    }

    for (col=0; col<BOX_MAP_WIDTH; col++) {
        for (line=0; line<BOX_MAP_HEIGHT; line++) {
            if (destroy_map->map[col][line] == 'D') {
                map->map[col][line] = ' ';
            }
        }
    }

    free(destroy_map);

    return changed;
}

int box_tick(struct box_map *map) {
    int block_count;
    char loop = 1;

     while (loop) {
        block_count = box_gravity(map);
        loop = box_destroy(map);
    }

    return block_count;
}

struct box_map *box_load(char *filename) {
    int car, line=0, col=0;
    struct box_map *map;
    FILE *file;

    file = fopen(filename, "r");

    if (file == NULL) {
        return NULL;
    }

    map = box_map_create();

    if (map == NULL) {
        return NULL;
    }

    while ((car = fgetc(file)) != EOF) {
        car = car & 0xff;

        if (car == '\r')
            continue;

        if (car == '\n') {
            line++;
            col = 0;
        } else {
            if (col < BOX_MAP_WIDTH && line < BOX_MAP_HEIGHT) {
                map->map[col++][line] = (car & 0xff);
            }
        }
    }

    box_tick(map);

    printf("Map loaded:\n");
    box_print(map);

    return map;
}

void box_map_copy(struct box_map *in, struct box_map *out) {
    int line, col;

    for (line=0; line<BOX_MAP_HEIGHT; line++) {
        for (col=0; col<BOX_MAP_WIDTH; col++) {
            out->map[col][line] = in->map[col][line];
        }
    }
}

char box_map_switch(struct box_map *map, int col, int line, int direction) {
    char b, a = map->map[col][line];

    if (direction) {
        if (col < BOX_MAP_WIDTH-1) { 
            b = map->map[col+1][line];
            if (a != b) {
                map->map[col][line] = b;
                map->map[col+1][line] = a;
                return 1;
            }
        }
    } else {
        if (line < BOX_MAP_HEIGHT-1) {
            b = map->map[col][line+1];

            if (a != b) {
                map->map[col][line] = b;
                map->map[col][line+1] = a;
                return 1;
            }
        }
    }

    return 0;
}

char box_solve_rec(struct box_map *map, int tries, char **buffer) {
    int line, col, direction;
    struct box_map *copy = box_map_create();

    box_tick(map);
    box_map_copy(map, copy);

    if (tries <= 0) {
        return 0;
    }

    for (line=0; line<BOX_MAP_HEIGHT; line++) {
        for (col=0; col<BOX_MAP_WIDTH; col++) {
            for (direction=0; direction<2; direction++) {
                box_map_copy(map, copy);

                if (box_map_switch(copy, col, line, direction)) {
                    if (box_tick(copy) == 0 || box_solve_rec(copy, tries-1, buffer)) {
                        sprintf(buffer[tries-1], "Switching col=%d, line=%d with col=%d, line=%d", 
                                col+1, line+1, col+1+direction, line+1+!direction);

                        free(copy);
                        return 1;
                    }
                }
            }
        }
    }

    free(copy);

    return 0;
}

char box_solve(struct box_map *map, int tries) {
    char **buffer;
    char success;
    int i;

    buffer = malloc(tries * sizeof(char *));

    for (i=0; i<tries; i++) {
        buffer[i] = malloc(512 * sizeof(char));
    }

    success = box_solve_rec(map, tries, buffer);

    if (success) {
        printf("Success !\n");
    } else {
        printf("No solution found\n");
    }

    for (i=0; i<tries; i++) {
        if (success) {
            printf("%d) %s\n", i+1, buffer[tries-i-1]);
        }
        free(buffer[tries-i-1]);
    }
    
    free(buffer);
}
