#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


void error_checking(int result, char* error)
{
    if (result == -1)
    {
        printf("%s\n", error);
        exit(-1);
    }
}

char* get_string()
{
    int string_length = 0;
    int capacity = 1;
    char* string = (char*) malloc(sizeof(char));
    char symbol = getchar();

    if (symbol == EOF)
    {
        return NULL;
    }

    if (symbol == '\n')
    {
        symbol = getchar();
    }

    while (symbol != '\n')
    {
        string[string_length] = symbol;
        ++string_length;
        
        if (string_length >= capacity)
        {
            capacity *= 2;
            string = (char*) realloc(string, capacity * sizeof(char));
        }

        symbol = getchar();
    }

    string[string_length] = '\0';
    return string;
}


int main()
{
    int first_pipe_fd[2];
    int second_pipe_fd[2];
    
    error_checking(pipe(first_pipe_fd), "Pipe creating error");
    error_checking(pipe(second_pipe_fd), "Pipe creating error");
    
    char* first_filename;
    first_filename = get_string();
    
    char* second_filename;
    second_filename = get_string();

    int first_file, second_file;
    
    error_checking(first_file = open(first_filename, O_WRONLY | O_CREAT, 0777), "File openning error");
    error_checking(second_file = open(second_filename, O_WRONLY | O_CREAT, 0777), "File openning error");

    pid_t first_process_id = fork();
    error_checking(first_process_id, "Fork error");
    
    if (first_process_id == 0)
    {
        pid_t second_process_id = fork();
        error_checking(second_process_id, "Fork error");
        
        if (second_process_id == 0)
        {
            //printf("Second child process\n");
            close(second_pipe_fd[1]);
            close(first_pipe_fd[0]);
            close(first_pipe_fd[1]);
            error_checking(dup2(second_pipe_fd[0], fileno(stdin)), "dup2 error");
            close(second_pipe_fd[0]);
            error_checking(dup2(second_file, fileno(stdout)), "dup2 error");
            close(second_file);
            char* const* argv = NULL;
            error_checking(execv("child.out", argv), "Execv error");   
		}
		
		else
		{
		    //printf("First child process\n");
		    close(first_pipe_fd[1]);
            close(second_pipe_fd[0]);
            close(second_pipe_fd[1]);
            error_checking(dup2(first_pipe_fd[0], fileno(stdin)), "dup2 error");
            close(first_pipe_fd[0]);
            error_checking(dup2(first_file, fileno(stdout)), "dup2 error");
            close(first_file);
            char* const* argv = NULL;
            error_checking(execv("child.out", argv), "Execv error");  
            
		}
    }
    
    else
    {
        //printf("Parent process\n");
        close(first_pipe_fd[0]);
        close(second_pipe_fd[0]);
        char* input_string;
        while ((input_string = get_string()) != NULL)
        {
            int string_length = strlen(input_string); 
            int random_number = (rand() % 10) + 1;
            if (random_number <= 8)
            {
                write(first_pipe_fd[1], &string_length, sizeof(int));
                write(first_pipe_fd[1], input_string, sizeof(char) * string_length);
            }
            
            else
            {
                write(second_pipe_fd[1], &string_length, sizeof(int));
                write(second_pipe_fd[1], input_string, sizeof(char) * string_length);
            }
        }
        close(first_pipe_fd[1]);
        close(second_pipe_fd[1]);
        close(first_file);
        close(second_file);
    }
    
    return 0;
}
