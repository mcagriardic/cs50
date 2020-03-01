#include <stdio.h>
#include <cs50.h>

bool isElementin_integer(int arr[], int array_size, int element)
{
    bool is_element_not_in = false;
    for (int i=0; i < array_size; i++)
    {
        if (element == arr[i])
        {
            is_element_not_in = true;
            break;
        }
    }
    return is_element_not_in;
}
