#include <stdio.h>
#include <string.h>

bool isEqual_string(string arr1[], string arr2[], int m, int n)
{
    bool is_equal = false;
    int equal_count=0;
    for (int i=0; i < m; i++)
    {
        for (int j=0; j < n; j++)
        {
            // printf("Comparing %s and %s ...\n",arr1[i], arr2[j]);
            if (strcmp(arr1[i], arr2[j]) == 0)
            {
                // printf("%s exists!\n\n", arr1[i]);
                equal_count++;
                break;
            }
            // else
            // {
            //     printf("%s does not exist!\n\n", arr1[i]);
            // }
        }
    }

    if (equal_count == m)
    {
        is_equal = true;
    }
    return is_equal;
}
