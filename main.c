#include <stdio.h>
#include <stdlib.h>

typedef struct color
{
    unsigned char r, g, b;
};

void addTgaHeader(FILE *ft, int width, int height);

void commitImage(FILE *ft, struct color *data, int width, int height);

void rect(color *data, int imgWidth, int imgHeight, int x, int y, int width, int height, color *fill);

int coordToOffset(int x, int y, int width, int height)
{
    return x * width + y;
}

int main(void)
{
    int const width = 100;
    int const height = 100;
    char const *name = "result.tga";

    FILE *ft;
    ft = fopen(name, "w");
    addTgaHeader(ft, width, height);

    color *ptr;
    ptr = (color*) malloc((width * height) * sizeof(color));

    int resolution = width * height;
    for(int i = 0; i <= resolution; i++)
    {
        (ptr + i)->r = 255;
        (ptr + i)->g = 0;
        (ptr + i)->b = 255;
    }

    color *white;
    white = (color*) malloc(sizeof(color));
    white->r = 255;
    white->g = 255;
    white->b = 255;

    color *red;
    red = (color*) malloc(sizeof(color));
    red->r = 255;
    red->g = 0;
    red->b = 0;

    rect(ptr, width, height, 50, 50, 50, 50, white);
    rect(ptr, width, height, 0, 49, 2, 2, red);

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
    for(int y = height - 1; y >= 0; y--)
    {
        for(int x = 0; x < width; x++)
        {
            putc((data + coordToOffset(x, y, width, height))->b, ft);
            putc((data + coordToOffset(x, y, width, height))->g, ft);
            putc((data + coordToOffset(x, y, width, height))->r, ft);
        }
    }
    //for(int i = 0; i < width * height; i++)
    //{
        //putc((data + i)->b, ft);
        //putc((data + i)->g, ft);
        //putc((data + i)->r, ft);
    //}
}

void rect(color *data, int imgWidth, int imgHeight, int x, int y, int width, int height, color *fill)
{
    for(int dx = x; dx < x + width; dx++)
    {
        for(int dy = y; dy < y + height; dy++)
        {
            (data + coordToOffset(dx, dy, imgWidth, imgHeight))->r = fill->r;
            (data + coordToOffset(dx, dy, imgWidth, imgHeight))->g = fill->g;
            (data + coordToOffset(dx, dy, imgWidth, imgHeight))->b = fill->b;
        }
    }
}

