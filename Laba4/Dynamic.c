#include <stdio.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <stdlib.h>

const char* path1 = "libfunctions1.so";
const char* path2 = "libfunctions2.so";
const char* PI_FUNC_NAME = "PI";
const char* E_FUNC_NAME = "E";

double (*PI)(int);
double (*E)(int);

void* handle = NULL;
int first = 1;

void load()
{
    char* path;
    
    if(first)
    {
        path = path1;
    }
    else
    {
        path = path2;
    }
    
    handle = dlopen(path, RTLD_LAZY);
    
    if(!handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
}

void unload()
{
    dlclose(handle);
}

void load_functions()
{
    load();
    
    PI = dlsym(handle, PI_FUNC_NAME);
    E = dlsym(handle, E_FUNC_NAME);
        
    char* error;
    if (error = dlerror())
    {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
}

void change()
{
    unload();
    first ^= 1;
    load_functions();
}

int main()
{
    load_functions();
    int symbol = 1;
    
    printf("Please choose option:\n 0.Change realisation\n 1.Calculating PI\n 2.Calculating E\n 3.Help\n 4.Exit\n");

    while (scanf("%d", &symbol) != EOF)
    {
        switch(symbol)
        {
            case 0:
            {
                change();
                printf("Changed\n");
                
                if (first)
                {
                    printf("Now it's first\n");
                }
                else
                {
                    printf("Now it's second\n");
                }

                break;
            }

            case 1:
            {
                int accuracy;
                printf("Input accuracy\n");
                scanf("%d", &accuracy);
                printf("%lf\n", PI(accuracy));
                break;
            }

            case 2:
            {
                int accuracy;
                printf("Input accuracy\n");
                scanf("%d", &accuracy);
                printf("%lf\n", E(accuracy));
                break;
            }

            case 3:
            {
                printf("Please choose option:\n 0.Change realisation\n 1.Calculating PI\n 2.Calculating E\n 3.Help\n 4.Exit\n");
                break;
            }

            case 4:
            {
                unload();
                return 0;
            }

            default:
            {
                printf("Wrong input, use 3 for help\n");
            }
        }   
    }
    unload();

    return 0;
}
