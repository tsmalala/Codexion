#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char    ft_tolower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return (c + 32);
    return (c);
}

void to_lower_string(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower((unsigned char)str[i]);
        i++;
    }
}

int    arg_validator(char *argv[])
{
    int i = 1;

    while (i <= 7)
    {
        int j = 0;
        long int n = 0;
        char *str = argv[i];

        while (str[j])
        {
            if (str[j] >= '0' && str[j] <= '9')
            {
                n = n * 10 + (str[j] - '0');
                j++;
            }
            else
                return 0;
        }
        if (n > 2147483647)
            return 0;
        i++;
    }
    return 1;
}

int error_handlers(int number_of_coders, char *scheduler, char *argv[])
{
    if (arg_validator(argv) == 0)
        return 0;
    to_lower_string(scheduler);
    if (number_of_coders < 0)
        return 0;
    if (strcmp(scheduler, "fifo") != 0 && strcmp(scheduler, "edf") != 0)
        return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc == 9)
    {
        int number_of_coders = atoi(argv[1]);
        int time_to_burnout = atoi(argv[2]);
        int time_to_compile = atoi(argv[3]);
        int time_to_debug = atoi(argv[4]);
        int time_to_refactor = atoi(argv[5]);
        int number_of_compiles_required = atoi(argv[6]);
        int dongle_cooldown = atoi(argv[7]);
        char *scheduler = argv[8];

        if (!error_handlers(number_of_coders, scheduler, argv))
        {
            printf("[Error]: An error occurred during data processing.\n");
            return 0;
        }
    }
    else
        printf("[Error]: Not enough arguments.");
    printf("\n");
    return 0;
}
