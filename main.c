#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159265

typedef struct
{
    unsigned char r, g, b;
}color;

enum tgaDataTypeCode {
    noImageData = 0,
    uncompressedColorMap = 1,
    uncompressedRgb = 2,
    uncompressedBlackAndWhite = 3,
    runLengthColorMap = 9,
    runLengthRgb = 10,
    compressedBlackAndWhite = 11,
    compressedColorMapHuffmanDeltaRunlength = 32,
    compressedColorMapHuffmanDeltaRunlengthFourPass = 33
};

// IMAGE
char const *imageName = "result-runlength.tga";
int const imageWidth = 800;
int const imageHeight = 600;

int resolution = imageWidth * imageHeight;
FILE *imageFile;
color *imagePixels;

// COLORS
color green = { 23, 167, 47 };
color *darkGreen;
color *white;
color *pureWhite;
color *yellow;
color *blue;

int coordToOffset(int x, int y)
{
    return y * imageWidth + x;
}

void fillImage(color *fill)
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
    //green = (struct color*) malloc(sizeof(struct color));
    //green->r = 23;
    //green->g = 167;
    //green->b = 47;

    darkGreen = (color*) malloc(sizeof(color));
    darkGreen->r = 9;
    darkGreen->g = 67;
    darkGreen->b = 19;

    white = (color*) malloc(sizeof(color));
    white->r = 234;
    white->g = 234;
    white->b = 234;

    yellow = (color*) malloc(sizeof(color));
    yellow->r = 255;
    yellow->g = 255;
    yellow->b = 0;

    pureWhite = (color*) malloc(sizeof(color));
    pureWhite->r = 255;
    pureWhite->g = 255;
    pureWhite->b = 255;

    blue = (color*) malloc(sizeof(color));
    blue->r = 0;
    blue->g = 153;
    blue->b = 204;
}

void freeColors()
{
    free(darkGreen);
    free(white);
    free(yellow);
    free(pureWhite);
}

void addTgaHeader()
{
    putc(0, imageFile);
    putc(0, imageFile);
    putc(runLengthRgb, imageFile);
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

bool colorsAreSame(color *a, color *b)
{
    if( a->r == b ->r &&
        a->g == b->g &&
        a->b == b->b)
    {
        return true;
    }
    return false;
}

void commitImage()
{
    char repLength = 0;
    color *currentColor;
    currentColor = (color*) malloc(sizeof(color));
    currentColor->b = (imagePixels + coordToOffset(0, imageHeight - 1))->b;
    currentColor->g = (imagePixels + coordToOffset(0, imageHeight - 1))->g;
    currentColor->r = (imagePixels + coordToOffset(0, imageHeight - 1))->r;

    for(int y = imageHeight - 1; y >= 0; y--)
    {
        for(int x = 0; x < imageWidth; x++)
        {
            bool isRepeating = false;
            if(colorsAreSame(currentColor, (imagePixels + coordToOffset(x, y))) && repLength < 127)
            {
                repLength++;
                if(x == imageWidth - 1 && y == 0)
                {
                    isRepeating = true;
                }
                else
                {
                    continue;
                }
            }

            // Either found a different color OR ran into a repetition limit OR the image ended
            // Write down the repetition and try again

            putc(repLength + 127, imageFile);
            putc(currentColor->b, imageFile);
            putc(currentColor->g, imageFile);
            putc(currentColor->r, imageFile);

            currentColor->b = (imagePixels + coordToOffset(x, y))->b;
            currentColor->g = (imagePixels + coordToOffset(x, y))->g;
            currentColor->r = (imagePixels + coordToOffset(x, y))->r;


            if(x == imageWidth - 1 && y == 0 && !isRepeating)
            {
                putc(128, imageFile);
                putc(currentColor->b, imageFile);
                putc(currentColor->g, imageFile);
                putc(currentColor->r, imageFile);
            }
            else
            {
                repLength = 1;
            }
        }
    }

    free(currentColor);
}

void rect(int x, int y, int width, int height, color *fill)
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

void point(int x, int y, color *fill)
{
    if(x < 0 || x > imageWidth || y < 0 || y > imageHeight) { return; }
    (imagePixels + coordToOffset(x, y))->r = fill->r;
    (imagePixels + coordToOffset(x, y))->g = fill->g;
    (imagePixels + coordToOffset(x, y))->b = fill->b;
}

void circle(int x, int y, int radius, color *fill)
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

void flower(int x, int y, int radius, color *fill)
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

    imagePixels = (color*) malloc(resolution * sizeof(color));

    fillImage(blue);

    /* Draw the grass */
    rect(0, 50, imageWidth, imageHeight - 50, &green);

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
