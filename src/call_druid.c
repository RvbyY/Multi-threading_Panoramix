/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** call_druid
*/

#include "../include/panoramix.h"
#include <unistd.h>
#include <stdlib.h>

void call_druid(druid_t *druid, thread_t *thread_manager)
{
    if (druid->is_called == false) {
        druid->is_called = true;
        sem_post(&thread_manager->call_druid);
    }
    thread_manager->nb_waiting++;
    pthread_mutex_unlock(&thread_manager->pot_mutex);
    sem_wait(&thread_manager->pot_refilled);
    pthread_mutex_lock(&thread_manager->pot_mutex);
}
