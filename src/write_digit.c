#include "write_digit.h"

void (*write_digit_to_bmp[9])(unsigned char *** bitmap, int x, int y) = 
{write_1, write_2, write_3, write_4, write_5, write_6, write_7, write_8, write_9};

void write_1 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if ( j == 4 )
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 0;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
            else
            {
                for (int k = 0; k < 3; k++)
                {
                    bitmap[bitmap_x + i][bitmap_y + j][k] = 255;
                }
            }
        }
    }
}

void write_2 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    int i,j;
    for(i = 0; i < 5; i++)
    {
        if (i == 0 || i == 2 || i == 4)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 0;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
        else if (i == 1)
        {
            for (j = 0; j < 4; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
            bitmap[bitmap_x + i][bitmap_y + 4][0] = 255;
            bitmap[bitmap_x + i][bitmap_y + 4][1] = 0;
            bitmap[bitmap_x + i][bitmap_y + 4][2] = 255;
        }
        else if (i == 3)
        {
            bitmap[bitmap_x + i][bitmap_y][0] = 255;
            bitmap[bitmap_x + i][bitmap_y][1] = 0;
            bitmap[bitmap_x + i][bitmap_y][2] = 255;
        
            for (j = 1; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    }
}

void write_3 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    int i,j;
    for (i = 0; i < 5; i += 2)
    {
        for(j = 0; j < 5; j++)
        {
            bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
            bitmap[bitmap_x + i][bitmap_y + j][1] = 0;
            bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
        }
    }
    for (i = 1; i < 5; i += 2)
    {
        for(j = 0; j < 4; j++)
        {
            bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
            bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
            bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
        }
        bitmap[bitmap_x + i][bitmap_y + 4][0] = 255;
        bitmap[bitmap_x + i][bitmap_y + 4][1] = 0;
        bitmap[bitmap_x + i][bitmap_y + 4][2] = 255;
    }
}

void write_4 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    int i,j;
    for(i = 0; i < 5; i++)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    for(j = 0; j < 5; j++)
    {
        bitmap[bitmap_x + 2][bitmap_y + j][0] = 255;
        bitmap[bitmap_x + 2][bitmap_y + j][1] = 0;
        bitmap[bitmap_x + 2][bitmap_y + j][2] = 255;
    }
    for(i = 0; i < 5; i++)
    {
        bitmap[bitmap_x + i][bitmap_y + 4][0] = 255;
        bitmap[bitmap_x + i][bitmap_y + 4][1] = 0;
        bitmap[bitmap_x + i][bitmap_y + 4][2] = 255;
    }
    for(i = 0; i < 3; i++)
    {
        bitmap[bitmap_x + i][bitmap_y][0] = 255;
        bitmap[bitmap_x + i][bitmap_y][1] = 0;
        bitmap[bitmap_x + i][bitmap_y][2] = 255;
    }
}

void write_5 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    int i,j;
    for(i = 0; i < 5; i++)
    {
        if (i == 0 || i == 2 || i == 4)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 0;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
        else if (i == 1)
        {
            for (j = 1; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
            bitmap[bitmap_x + i][bitmap_y][0] = 255;
            bitmap[bitmap_x + i][bitmap_y][1] = 0;
            bitmap[bitmap_x + i][bitmap_y][2] = 255;
        }
        else if (i == 3)
        {
            bitmap[bitmap_x + i][bitmap_y + 4][0] = 255;
            bitmap[bitmap_x + i][bitmap_y + 4][1] = 0;
            bitmap[bitmap_x + i][bitmap_y + 4][2] = 255;
        
            for (j = 0; j < 4; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    }
}

void write_6 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    int i,j;
    for(i = 0; i < 5; i++)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    for(i = 0; i < 5; i++)
    {
        if (i == 0 || i == 2 || i == 4)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 0;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    }
    bitmap[bitmap_x + 1][bitmap_y][0] = 255;
    bitmap[bitmap_x + 1][bitmap_y][1] = 0;
    bitmap[bitmap_x + 1][bitmap_y][2] = 255;

    bitmap[bitmap_x + 3][bitmap_y][0] = 255;
    bitmap[bitmap_x + 3][bitmap_y][1] = 0;
    bitmap[bitmap_x + 3][bitmap_y][2] = 255;

    bitmap[bitmap_x + 3][bitmap_y + 4][0] = 255;
    bitmap[bitmap_x + 3][bitmap_y + 4][1] = 0;
    bitmap[bitmap_x + 3][bitmap_y + 4][2] = 255;
}

void write_7 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    int i,j;
    for(i = 0; i < 5; i++)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    for(i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 0;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
        else
        {
                bitmap[bitmap_x + i][bitmap_y + 4][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + 4][1] = 0;
                bitmap[bitmap_x + i][bitmap_y + 4][2] = 255;
        }
    }
}

void write_8 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    int i,j;
    for(i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
            bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
            bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
        }
    }
    for(i = 0; i < 5; i++)
    {
        if (i == 0 || i == 2 || i == 4)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 0;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    }
    bitmap[bitmap_x + 1][bitmap_y][0] = 255;
    bitmap[bitmap_x + 1][bitmap_y][1] = 0;
    bitmap[bitmap_x + 1][bitmap_y][2] = 255;

    bitmap[bitmap_x + 1][bitmap_y + 4][0] = 255;
    bitmap[bitmap_x + 1][bitmap_y + 4][1] = 0;
    bitmap[bitmap_x + 1][bitmap_y + 4][2] = 255;

    bitmap[bitmap_x + 3][bitmap_y][0] = 255;
    bitmap[bitmap_x + 3][bitmap_y][1] = 0;
    bitmap[bitmap_x + 3][bitmap_y][2] = 255;

    bitmap[bitmap_x + 3][bitmap_y + 4][0] = 255;
    bitmap[bitmap_x + 3][bitmap_y + 4][1] = 0;
    bitmap[bitmap_x + 3][bitmap_y + 4][2] = 255;
}

void write_9 (unsigned char *** bitmap, int x, int y )
{
    int bitmap_x = 8 * x + 2;
    int bitmap_y = 8 * y + 2;
    int i,j;
    for(i = 0; i < 5; i++)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    for(i = 0; i < 5; i++)
    {
        if (i == 0 || i == 2 || i == 4)
        {
            for (j = 0; j < 5; j++)
            {
                bitmap[bitmap_x + i][bitmap_y + j][0] = 255;
                bitmap[bitmap_x + i][bitmap_y + j][1] = 0;
                bitmap[bitmap_x + i][bitmap_y + j][2] = 255;
            }
        }
    }
    bitmap[bitmap_x + 1][bitmap_y][0] = 255;
    bitmap[bitmap_x + 1][bitmap_y][1] = 0;
    bitmap[bitmap_x + 1][bitmap_y][2] = 255;

    bitmap[bitmap_x + 1][bitmap_y + 4][0] = 255;
    bitmap[bitmap_x + 1][bitmap_y + 4][1] = 0;
    bitmap[bitmap_x + 1][bitmap_y + 4][2] = 255;
    
    bitmap[bitmap_x + 3][bitmap_y + 4][0] = 255;
    bitmap[bitmap_x + 3][bitmap_y + 4][1] = 0;
    bitmap[bitmap_x + 3][bitmap_y + 4][2] = 255;
}

void write_failure (unsigned char *** bitmap)
{
    int i, j, m, n;
    for(i = 2; i < 73 - 2; i += 8)
    {
        for (j = 2; j < 73 - 2; j += 8)
        {
            for(m = 0; m < 5; m++)
            {
                for(n = 0; n < 5; n++)
                {
                    bitmap[i + m][j + n][0] = 255;
                    bitmap[i + m][j + n][1] = 255;
                    bitmap[i + m][j + n][2] = 255;
                }
            }
        }
    }
    for(i = 2; i < 73 - 2; i += 8)
    {
        for (j = 2; j < 73 - 2; j += 8)
        {
            bitmap[i][j][0] = 0; 
            bitmap[i][j][1] = 0; 
            bitmap[i][j][2] = 255;

            bitmap[i][j + 4][0] = 0; 
            bitmap[i][j + 4][1] = 0; 
            bitmap[i][j + 4][2] = 255;

            bitmap[i + 1][j + 1][0] = 0; 
            bitmap[i + 1][j + 1][1] = 0; 
            bitmap[i + 1][j + 1][2] = 255;

            bitmap[i + 1][j + 3][0] = 0; 
            bitmap[i + 1][j + 3][1] = 0; 
            bitmap[i + 1][j + 3][2] = 255;

            bitmap[i + 2][j + 2][0] = 0; 
            bitmap[i + 2][j + 2][1] = 0; 
            bitmap[i + 2][j + 2][2] = 255;

            bitmap[i + 3][j + 1][0] = 0; 
            bitmap[i + 3][j + 1][1] = 0; 
            bitmap[i + 3][j + 1][2] = 255;

            bitmap[i + 3][j + 3][0] = 0; 
            bitmap[i + 3][j + 3][1] = 0; 
            bitmap[i + 3][j + 3][2] = 255;

            bitmap[i + 4][j][0] = 0; 
            bitmap[i + 4][j][1] = 0; 
            bitmap[i + 4][j][2] = 255;

            bitmap[i + 4][j + 4][0] = 0; 
            bitmap[i + 4][j + 4][1] = 0; 
            bitmap[i + 4][j + 4][2] = 255;
        }
    }
}