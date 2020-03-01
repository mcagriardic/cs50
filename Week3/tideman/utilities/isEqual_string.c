#include <stdio.h>
#include <cs50.h>
#include <string.h>

bool isEqual_string(string arr1[], string arr2[], int m, int n);

int main()
{
    string arr1[3] = {"Bob", "Charlie", "Alice"};
    string arr2[3] = {"Alice", "Bob", "Charlie"};
    bool is_equal;

    is_equal = isEqual(arr1, arr2, 3, 3);
    printf("\n\n*******%d******\n\n", is_equal);
}

bool isEqual(string arr1[], string arr2[], int m, int n)
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
                // printf("%s does not exist!\n\n", arr1[i]);
            // }
        }
    }

    if (equal_count == m)
    {
        is_equal = true;
    }
    return is_equal;
}
