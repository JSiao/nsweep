#pragma once

struct board
{
    unsigned char height, width;
    char **data;
};

struct board * board_create(int, int);
void board_destroy(struct board **);

void board_check(struct board *, int, int);

