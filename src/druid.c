/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** druid
*/

#include "../include/panoramix.h"
#include <unistd.h>

void clear_druid(druid_t *druid)
{
    if (!druid)
        return;
    write(1, "Druid: I'm out of viscum. I'm going back to... zZz\n", 52);
}

void *druid_thread(void *args)
{
    arg_druid_t *arg = (arg_druid_t *)args;
    size_t to_wake = 0;

    while (arg->druid->nb_refills > 0) {
        sem_wait(&arg->thread_manager->call_druid);
        pthread_mutex_lock(&arg->thread_manager->pot_mutex);
        refill_pot(arg->druid);
        to_wake = arg->thread_manager->nb_waiting;
        pthread_mutex_unlock(&arg->thread_manager->pot_mutex);
        for (size_t i = 0; i < to_wake; i++)
            sem_post(&arg->thread_manager->pot_refilled);
        arg->thread_manager->nb_waiting = 0;
    }
    sem_wait(&arg->thread_manager->druid_can_exit);
    clear_druid(arg->druid);
    free(arg);
    pthread_exit(EXIT_SUCCESS);
}

druid_t *build_a_new_druid(size_t pot_size, size_t nb_refill)
{
    druid_t *druid = malloc(sizeof(druid_t));

    if (!pot_size || !nb_refill)
        return NULL;
    druid->nb_refills = nb_refill;
    druid->pot_size = pot_size;
    druid->pot = pot_size;
    druid->is_called = false;
    write(1, "Druid: I'm ready... but sleepy...\n", 35);
    return druid;
}

size_t refill_pot(druid_t *druid)
{
    if (!druid->nb_refills || druid->pot)
        return druid->pot;
    druid->pot = druid->pot_size;
    druid->nb_refills--;
    printf("Druid: Ah! Yes, yes, I'm awake! Working on it! Beware I can only make %ld more refills after this one.\n", druid->nb_refills);
    return druid->pot;
}
