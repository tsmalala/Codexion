#include <pthread.h>
#include "coder_routine.h"
#include "burnout.h"
#include "main.h"

void    run_threads(Coder **coders, Info *data)
{
    int i = 0;
    int i_join = 0;

    pthread_t burnout_thread;
    pthread_create(&burnout_thread, NULL, check_burnout, coders);
    
    while (i < data->number_coders)
    {
        pthread_create(&coders[i]->pthread, NULL, coder_routine, coders[i]);
        i++;
    }
    pthread_join(burnout_thread, NULL);
    while (i_join < data->number_coders)
    {
        pthread_join(coders[i_join]->pthread, NULL);
        i_join++;
    }
}
