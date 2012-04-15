#include <stdio.h>
#include <stdlib.h>
#include "box.h"

int main(int argc, char *argv[])
{
    struct box_map *map;

    if (argc != 3) {
        fprintf(stderr, "Usage: ./box <file.box> <tries>\n");
        return EXIT_FAILURE;
    }

    map = box_load(argv[1]);

    box_solve(map, atoi(argv[2]));

    return EXIT_SUCCESS;
}
