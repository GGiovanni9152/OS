#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


int is_vowel(char symbol)
{
    symbol = tolower(symbol);
    
    if (symbol == 'a' || symbol == 'e' || symbol == 'i' || symbol == 'o' || symbol == 'u' || symbol == 'y')
    {
        return 1;
    }
    
    return 0;
}

char* delete_vowels(char input_string[], int input_string_size)
{
    int output_string_size = 0;
    int capacity = 1;
    char* output_string = (char*) malloc(sizeof(char));
    
    for (int index = 0; index < input_string_size; ++index)
    {
        if (!is_vowel(input_string[index]))
        {
            output_string[output_string_size] = input_string[index];
            ++output_string_size;
            
            if (output_string_size >= capacity)
            {
                capacity *= 2;
                output_string = (char*) realloc(output_string, capacity * sizeof(char));
            }
        }
    }
    output_string[output_string_size] = '\0';
    return output_string;
}


int main()
{

    int input_size;
    while (read(fileno(stdin), &input_size, sizeof(int)) != 0)
    {
        char input_string[input_size];
        read(fileno(stdin), &input_string, sizeof(char) * input_size);
        char* new_string;
        new_string = delete_vowels(input_string, input_size);
        write(fileno(stdout), new_string, sizeof(char) * strlen(new_string));
        write(fileno(stdout), "\n", sizeof(char));
    }
    
    
    return 0;
}
