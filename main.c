#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

typedef struct color
{
    unsigned char r, g, b;
};

void addTgaHeader(FILE *ft, int width, int height);

void commitImage(FILE *ft, struct color *data, int width, int height);

void rect(color *data, int imgWidth, int imgHeight, int x, int y, int width, int height, color *fill);

void circle(color *data, int imgWidth, int imgHeight, int x, int y, int radius, color *fill);

void flower(color *data, int imgWidth, int imgHeight, int x, int y, int radius, color *fill);

void point(color *data, int imgWidth, int imgHeight, int x, int y, color *fill);

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
        (ptr + i)->r = 0;
        (ptr + i)->g = 153;
        (ptr + i)->b = 204;
    }

    color *green;
    green = (color*) malloc(sizeof(color));
    green->r = 23;
    green->g = 167;
    green->b = 47;

    color *darkGreen;
    darkGreen = (color*) malloc(sizeof(color));
    darkGreen->r = 9;
    darkGreen->g = 67;
    darkGreen->b = 19;

    color *white;
    white = (color*) malloc(sizeof(color));
    white->r = 234;
    white->g = 234;
    white->b = 234;

    color *yellow;
    yellow = (color*) malloc(sizeof(color));
    yellow->r = 255;
    yellow->g = 255;
    yellow->b = 0;

    color *pureWhite;
    pureWhite = (color*) malloc(sizeof(color));
    pureWhite->r = 255;
    pureWhite->g = 255;
    pureWhite->b = 255;

    rect(ptr, width, height, 0, 50, width, 50, green);

    rect(ptr, width, height, 39, 67, 1, 20, darkGreen);
    flower(ptr, width, height, 40, 67, 10, white);

    flower(ptr, width, height, 50, 25, 10, pureWhite);
    flower(ptr, width, height, 50 + 15, 25, 10, pureWhite);
    flower(ptr, width, height, 50 + 30, 25, 10, pureWhite);

    flower(ptr, width, height, 4, 4, 40, yellow);

    commitImage(ft, ptr, width, height);

    fclose(ft);
    free(ptr);
    free(green);
    free(darkGreen);
    free(white);
    free(yellow);
    free(pureWhite);
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

void circle(color *data, int imgWidth, int imgHeight, int x, int y, int radius, color *fill)
{
    int twoRadius = 2 * radius;
    double end = 2 * PI;
    double stepSize = end / 50;

    double angle = 0;
    double px = (sin(angle) * radius) + x;
    double py = (-cos(angle) * radius) + y;
    while(angle <= end)
    {
        point(data, imgWidth, imgHeight, px, py, fill);

        angle = angle + stepSize;

        px = (sin(angle) * radius) + x;
        py = (-cos(angle) * radius) + y;
    }
}

void flower(color *data, int imgWidth, int imgHeight, int x, int y, int radius, color *fill)
{
    for(int i = 1; i <= radius; i++)
    {
        circle(data, imgWidth, imgHeight, x, y, i, fill);
    }
}

void point(color *data, int imgWidth, int imgHeight, int x, int y, color *fill)
{
    if(x < 0 || x > imgWidth || y < 0 || y > imgHeight) { return; }
    (data + coordToOffset(x, y, imgWidth, imgHeight))->r = fill->r;
    (data + coordToOffset(x, y, imgWidth, imgHeight))->g = fill->g;
    (data + coordToOffset(x, y, imgWidth, imgHeight))->b = fill->b;
}

