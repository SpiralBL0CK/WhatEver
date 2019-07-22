#include <stdlib.h>

unsigned int partition(unsigned int n)
{
    unsigned int i, j;
    unsigned int result;
    unsigned int **table;

    if(n == 0)
    {
        return 1;
    }

    table = malloc((n+1) * sizeof(unsigned int *));
    for(i=0;i<= n;i++)
    {
        table[i] = malloc((n+1) * sizeof(unsigned int));
    }
    for (i = 0;i <= n; i++) {
        table[i][0]=0;
    }
    for (i = 1; i <= n; i++) {
        table[0][i] = 1;
    }
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            if(j > i)
            {
                table[i][j] = table[i][j - 1];
            }
            else {
                table[i][j] = table[i][j - 1] + table[i - j][j];
            }
        }
    }
    for (i = 0; i <= n; i++) {
        free(table[i]);
    }
    free(table);
 
    return result;
}