#include<stdio.h>

#define ROWS 3
#define COLS 4
//变长形参
int sum2d(int rows, int cols, int ar[rows][cols])
{
        int i, j, sum;
        sum = 0;

        for(i = 0; i < rows; i++)
            for(j = 0; j < cols; j++)
                sum += ar[i][j];
        return sum;
}
int variable_length_array(int rs, int cs)
{
    int i, j;
    int varr[rs][cs]; //变长数组
    for(i = 0; i < rs; i++)                                                      
        for(j = 0; j < cs; j++)                                                  
            varr[i][j] = i * j +j;                                               
    printf("%d * %d array\n", rs, cs);
    printf("sum of all elemts = %d\n", sum2d(rs, cs, varr));
}
int main()
{
    int i, j;
    int junk[ROWS][COLS] = {
        {2, 4, 6, 8},
        {3, 5, 7, 9},
        {12, 10, 8, 6}
    };

    int morejunk[ROWS-1][COLS+2] = {
        {20, 30, 40, 50, 60, 70},
        {5, 6, 7, 8, 9, 10}
    };
    int rs = 5, cs = 2;
    variable_length_array(rs, cs);

    int varr[rs][cs]; //变长数组
   
    for(i = 0; i < rs; i++)
        for(j = 0; j < cs; j++)
            varr[i][j] = i * j +j;
    printf("3 * 5 array\n");
    printf("sum of all elemts = %d\n",sum2d(ROWS, COLS, junk));

    printf("2 * 6 array\n");
    printf("sum of all elemts = %d\n",sum2d(ROWS - 1, COLS + 2, morejunk));

    printf("3 * 10 array\n");
    printf("sum of all elemts = %d\n",sum2d(rs, cs, varr));

    return 0;
}
