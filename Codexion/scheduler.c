#include <stdio.h>
#include <string.h>
#include "main.h"
#include "time_utils.h"

int fifo(Coder *coder, Coder *neighbor)
{
    if (neighbor->is_compiling)
        return 1;
    return get_time(coder->time_request) > get_time(neighbor->time_request);
}

int scheduler(Coder *coder)
{
    if (strcmp(coder->data->scheduler, "fifo") == 0)
    {
        if (!coder->left_dongle->is_free || !coder->right_dongle->is_free)
        {
            coder->is_compiling = 1;
            return 1;
        }
        if (fifo(coder, coder->left_coder)
            && fifo(coder, coder->right_coder))
            return 0;
        return 1;
    }
    if (strcmp(coder->data->scheduler, "edf") == 0)
    {
        printf("EDF\n");
    }
    return 0;
}