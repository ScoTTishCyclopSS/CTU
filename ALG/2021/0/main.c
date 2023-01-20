/*
 * autor: @timusfed
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_SIZE 1000
#define NOT_POSSIBLE_NUM -1;

int main(int argc, char *argv[]) {
    int in_num, r, counter = 0;
    bool is_repeat = false;

    int *input_arr = (int*)malloc(sizeof(int) * INPUT_SIZE);
    
    // input
    while((r = scanf("%d", &in_num)) == 1)
    {
        input_arr[counter++] = in_num;
    }

    // bubble sorting
    for (int i = counter - 1; i >= 0; i--)
    {
        for (int k = 0; k < i; k++)
        {
            if (input_arr[k] > input_arr[k + 1])
            {
                int swap = input_arr[k];
                input_arr[k] = input_arr[k + 1];
                input_arr[k + 1] = swap;
            }
        }
    }

    // finding repeat numbers
    for(int i = 0; i < counter; i++)
    {
        if(input_arr[i] >= 0)
        {
            for(int l = i + 1; l < counter; l++)
            {
                if(input_arr[l] == input_arr[i])
                {
                    input_arr[l] = NOT_POSSIBLE_NUM;
                    is_repeat = true;
                }
            }

            if(is_repeat)
            {
                printf(" %d", input_arr[i]);
                is_repeat = false;
            }
        }
    }
    free(input_arr);
    printf("\n");
}
