#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

struct color
{
    unsigned char r, g, b;
};

char const *imageName = "result.tga";
int const imageWidth = 100;
int const imageHeight = 100;

int resolution = imageWidth * imageHeight;
FILE *imageFile;
struct color *imagePixels;

// COLORS
struct color *green;
struct color *darkGreen;
struct color *white;
struct color *pureWhite;
struct color *yellow;
struct color *blue;

int coordToOffset(int x, int y)
{
    return x * imageWidth + y;
}

void fillImage(struct color *fill)
{
    for(int i = 0; i <= resolution; i++)
    {
        (imagePixels + i)->r = fill->r;
        (imagePixels + i)->g = fill->g;
        (imagePixels + i)->b = fill->b;
    }
}

void loadColors()
{
    green = (struct color*) malloc(sizeof(struct color));
    green->r = 23;
    green->g = 167;
    green->b = 47;

    darkGreen = (struct color*) malloc(sizeof(struct color));
    darkGreen->r = 9;
    darkGreen->g = 67;
    darkGreen->b = 19;

    white = (struct color*) malloc(sizeof(struct color));
    white->r = 234;
    white->g = 234;
    white->b = 234;

    yellow = (struct color*) malloc(sizeof(struct color));
    yellow->r = 255;
    yellow->g = 255;
    yellow->b = 0;

    pureWhite = (struct color*) malloc(sizeof(struct color));
    pureWhite->r = 255;
    pureWhite->g = 255;
    pureWhite->b = 255;

    blue = (struct color*) malloc(sizeof(struct color));
    blue->r = 0;
    blue->g = 153;
    blue->b = 204;
}

void freeColors()
{
    free(green);
    free(darkGreen);
    free(white);
    free(yellow);
    free(pureWhite);
}

void addTgaHeader()
{
    putc(0, imageFile);
    putc(0, imageFile);
    putc(2, imageFile);                     /* uncompressed RGB */
    putc(0, imageFile); putc(0, imageFile);
    putc(0, imageFile); putc(0, imageFile);
    putc(0, imageFile);
    putc(0, imageFile); putc(0, imageFile); /* X origin */
    putc(0, imageFile); putc(0, imageFile); /* y origin */

    putc((imageWidth & 0x00FF), imageFile);
    putc((imageWidth & 0xFF00) / 256, imageFile);

    putc((imageHeight & 0x00FF), imageFile);
    putc((imageHeight & 0xFF00) / 256, imageFile);

    putc(24, imageFile); /* 24 bit bitmap */
    putc(0, imageFile);
}

void commitImage()
{
    for(int y = imageHeight - 1; y >= 0; y--)
    {
        for(int x = 0; x < imageWidth; x++)
        {
            putc((imagePixels + coordToOffset(x, y))->b, imageFile);
            putc((imagePixels + coordToOffset(x, y))->g, imageFile);
            putc((imagePixels + coordToOffset(x, y))->r, imageFile);
        }
    }
}

void rect(int x, int y, int width, int height, struct color *fill)
{
    for(int dx = x; dx < x + width; dx++)
    {
        for(int dy = y; dy < y + height; dy++)
        {
            (imagePixels + coordToOffset(dx, dy))->r = fill->r;
            (imagePixels + coordToOffset(dx, dy))->g = fill->g;
            (imagePixels + coordToOffset(dx, dy))->b = fill->b;
        }
    }
}

void point(int x, int y, struct color *fill)
{
    if(x < 0 || x > imageWidth || y < 0 || y > imageHeight) { return; }
    (imagePixels + coordToOffset(x, y))->r = fill->r;
    (imagePixels + coordToOffset(x, y))->g = fill->g;
    (imagePixels + coordToOffset(x, y))->b = fill->b;
}

void circle(int x, int y, int radius, struct color *fill)
{
    double end = 2 * PI;
    double stepSize = end / 50;

    double angle = 0;
    double px = (sin(angle) * radius) + x;
    double py = (-cos(angle) * radius) + y;

    while(angle <= end)
    {
        point((int)px, (int)py, fill);

        angle = angle + stepSize;

        px = (sin(angle) * radius) + x;
        py = (-cos(angle) * radius) + y;
    }
}

void flower(int x, int y, int radius, struct color *fill)
{
    for(int i = 1; i <= radius; i++)
    {
        circle(x, y, i, fill);
    }
}

int main(void)
{
    imageFile = fopen(imageName, "w");
    addTgaHeader();
    loadColors();

    imagePixels = (struct color*) malloc(resolution * sizeof(struct color));

    fillImage(blue);

    /* Draw the grass */
    rect(0, 50, imageWidth, 50, green);

    /* Draw the flower */
    rect(39, 67, 1, 20, darkGreen);
    flower(40, 67, 10, white);

    /* Draw the cloud */
    flower(50, 25, 10, pureWhite);
    flower(50 + 15, 25, 10, pureWhite);
    flower(50 + 30, 25, 10, pureWhite);

    /* Draw the Sun */
    flower(4, 4, 40, yellow);

    /* Save Image */
    commitImage();
    fclose(imageFile);

    /* Free memory */
    free(imageFile);
    freeColors();

    return 0;
}

