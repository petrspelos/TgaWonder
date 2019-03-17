#include <stdio.h>

void addTgaHeader(FILE *ft, int width, int height);

void addYellowBlob(FILE *ft, int width, int height);

int main(void)
{
    int const width = 100;
    int const height = 100;
    char const *name = "result.tga";

    FILE *ft;
    ft = fopen(name, "w");
    addTgaHeader(ft, width, height);
    addYellowBlob(ft, width, height);
    fclose(ft);
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

void addYellowBlob(FILE *ft, int width, int height)
{
    for(int i = 0; i < width * height; i++)
    {
        putc(255, ft);
        putc(0, ft);
        putc(255, ft);
    }
}

