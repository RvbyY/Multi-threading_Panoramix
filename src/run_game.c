/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** run_game
*/

#include "../include/panoramix.h"
#include <unistd.h>
#include <stdlib.h>

static void init_semaphores(thread_t *thread_manager, sem_t *semaphore, size_t nb_villagers)
{
    pthread_mutex_init(&thread_manager->pot_mutex, NULL);
    sem_init(semaphore, PTHREAD_PROCESS_SHARED, 1);
    sem_init(&thread_manager->call_druid, 0, 0);
    sem_init(&thread_manager->pot_refilled, 0, 0);
    sem_init(&thread_manager->druid_can_exit, 0, 0);
    thread_manager->nb_waiting = 0;
    thread_manager->active_villagers = nb_villagers;
}

static bool create_druid_thread(pthread_t *threads, druid_t *druid, size_t pot_size, size_t nb_refills, thread_t *tm)
{
    arg_druid_t *arg = malloc(sizeof(arg_druid_t));

    arg->druid = druid;
    arg->nb_refills = nb_refills;
    arg->pot_size = pot_size;
    arg->thread_manager = tm;
    if (pthread_create(&threads[0], NULL, druid_thread, arg) != 0) {
        free(arg);
        return false;
    }
    return true;
}

static bool call_villager_thread(pthread_t *threads, size_t idx, size_t nb_villagers, size_t nb_fights, druid_t *druid, thread_t *tm)
{
    arg_vilager_t *args = malloc(sizeof(arg_vilager_t));

    args->id = nb_villagers - idx;
    args->nb_fights = nb_fights;
    args->druid = druid;
    args->thread_manager = tm;
    if (pthread_create(&threads[idx], NULL, villager_thread, args) != 0) {
        free(args);
        return false;
    }
    return true;
}

static bool create_all_villager_threads(pthread_t *threads, size_t nb_villagers, size_t nb_fights, druid_t *druid, thread_t *tm)
{
    for (size_t i = 1; i <= nb_villagers; i++) {
        if (!call_villager_thread(threads, i, nb_villagers, nb_fights, druid, tm))
            return false;
    }
    return true;
}

static void wait_for_all_threads(pthread_t *threads, size_t nb_villagers)
{
    for (size_t i = 0; i <= nb_villagers; i++)
        pthread_join(threads[i], NULL);
}

static void cleanup_semaphores(thread_t *thread_manager, sem_t *semaphore)
{
    sem_destroy(semaphore);
    sem_destroy(&thread_manager->call_druid);
    sem_destroy(&thread_manager->pot_refilled);
    sem_destroy(&thread_manager->druid_can_exit);
    pthread_mutex_destroy(&thread_manager->pot_mutex);
}

bool run_game(unsigned int nb_villagers, unsigned int pot_size,
    unsigned int nb_fights, unsigned int nb_refills)
{
    sem_t semaphore;
    pthread_t threads[nb_villagers + 1];
    thread_t *thread_manager = malloc(sizeof(thread_t));
    druid_t *druid = build_a_new_druid(pot_size, nb_refills);

    if (!druid || !thread_manager)
        return false;
    init_semaphores(thread_manager, &semaphore, nb_villagers);
    if (!create_druid_thread(threads, druid, pot_size, nb_refills, thread_manager) || !create_all_villager_threads(threads, nb_villagers, nb_fights, druid, thread_manager)) {
        cleanup_semaphores(thread_manager, &semaphore);
        free(druid);
        free(thread_manager);
        return false;
    }
    wait_for_all_threads(threads, nb_villagers);
    cleanup_semaphores(thread_manager, &semaphore);
    free(druid);
    free(thread_manager);
    return true;
}