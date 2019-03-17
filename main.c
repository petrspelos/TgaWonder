#include <stdio.h>
#include <stdlib.h>

struct color
{
    unsigned char r, g, b;
};

void addTgaHeader(FILE *ft, int width, int height);

void commitImage(FILE *ft, struct color *data, int width, int height);

int main(void)
{
    int const width = 100;
    int const height = 100;
    char const *name = "result.tga";

    FILE *ft;
    ft = fopen(name, "w");
    addTgaHeader(ft, width, height);

    struct color *ptr;
    ptr = (struct color*) malloc((width * height) * sizeof(struct color));

    int resolution = width * height;
    for(int i = 0; i < resolution; i++)
    {
        (ptr + i)->r = 255;
        (ptr + i)->g = 0;
        (ptr + i)->b = 0;
    }

    commitImage(ft, ptr, width, height);

    fclose(ft);
    free(ptr);
    return 0;
}

void addTgaHeader(FILE *ft, int width, int height)
{
    putc(0,ft);
    putc(0,ft);
    putc(2,ft);                         /* uncompressed RGB */
    putc(0,ft); putc(0,ft);
    putc(0,ft); putc(0,ft);
    putc(0,ft);
    putc(0,ft); putc(0,ft);           /* X origin */
    putc(0,ft); putc(0,ft);           /* y origin */
    putc((width & 0x00FF),ft);
    putc((width & 0xFF00) / 256,ft);
    putc((height & 0x00FF),ft);
    putc((height & 0xFF00) / 256,ft);
    putc(24,ft);                        /* 24 bit bitmap */
    putc(0,ft);
}

void commitImage(FILE *ft, struct color *data, int width, int height)
{
    for(int i = 0; i < width * height; i++)
    {
        putc((data + i)->b, ft);
        putc((data + i)->g, ft);
        putc((data + i)->r, ft);
    }
}

