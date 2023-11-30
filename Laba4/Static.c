#include <stdio.h>

extern double PI(int);
extern double E(int);


int main()
{
    int symbol;
    printf("Please choose option:\n 1.Calculating PI\n 2.Calculating E\n 3.Help\n 4.Exit\n");

    while(scanf("%d", &symbol) > 0)
    {
        switch(symbol)
        {
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
                printf("Please choose option:\n 1.Calculating PI\n 2.Calculating E\n 3.Help\n 4.Exit\n");
                break;
            }

            case 4:
            {
                return 0;
            }

            default:
            {
                printf("Wrong input, use 3 for help\n");
            }
        }
        
    }

    return 0;
}

