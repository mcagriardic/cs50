#include <stdio.h>
#include <cs50.h>

bool isElementin_integer(int arr[], int array_size, int element);

int main()
{
    bool is_element_not_in;
    int element = -1;
    int arr[5] = {1,3,5,7,3};

    is_element_not_in = isElementin_integer(arr, 5, element);
    printf("Element %i is %d in ...\n", element, is_element_not_in);
    printf("%d---", true);
}

bool isElementin_integer(int arr[], int array_size, int element)
{
    bool is_element_not_in = false;
    for (int i=0; i < array_size; i++)
    {
        if (element == arr[i])
        {
            is_element_not_in = true;
        }
    }
    return is_element_not_in;
}
