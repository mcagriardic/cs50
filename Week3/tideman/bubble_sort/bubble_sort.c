# include <stdio.h>
# include <cs50.h>

int main()
{
    int arr[4] = {1,2,5,3};
    int temp;
    for (int i=0; i < 4-1; i++)
    {
        for (int j=0; j < 4-i-1; j++)
        {
            if (arr[j] < arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }

    for (int i=0; i < 4; i++)
    {
        printf("%i ", arr[i]);
    }
    printf("\n");
}
