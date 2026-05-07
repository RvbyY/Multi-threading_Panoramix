/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** main
*/

#include "../include/panoramix.h"
#include <unistd.h>
#include <stdlib.h>

void helper(void)
{
    write(1, "USAGE: ./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>\n", 70);
}

bool parse_values(unsigned int *nb_villagers, unsigned int *pot_size,
    unsigned int *nb_fights, unsigned int *nb_refills, char **argv)
{
    for (size_t i = 1; argv[i] != NULL; i++)
        if (atoi(argv[i]) <= 0)
            return false;
    *nb_villagers = (unsigned int)atoi(argv[1]);
    *pot_size = (unsigned int)atoi(argv[2]);
    *nb_fights = (unsigned int)atoi(argv[3]);
    *nb_refills = (unsigned int)atoi(argv[4]);
    return true;
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

void *villager_thread(void *arg)
{
    arg_vilager_t *args = (arg_vilager_t *)arg;
    villager_t *villager = build_a_new_villager(args->nb_fights, args->id);

    while (villager->nb_fights > 0) {
        pthread_mutex_lock(&args->thread_manager->pot_mutex);
        try_to_drink(villager, args->druid, args->thread_manager);
        pthread_mutex_unlock(&args->thread_manager->pot_mutex);
        villager_fight(villager);
    }
    pthread_mutex_lock(&args->thread_manager->pot_mutex);
    args->thread_manager->active_villagers--;
    if (args->thread_manager->active_villagers == 0)
        sem_post(&args->thread_manager->druid_can_exit);
    pthread_mutex_unlock(&args->thread_manager->pot_mutex);
    clear_villager(villager);
    free(args);
    pthread_exit(EXIT_SUCCESS);
}

bool run_game(unsigned int nb_villagers, unsigned int pot_size,
    unsigned int nb_fights, unsigned int nb_refills)
{
    sem_t semaphore;
    pthread_t threads[nb_villagers + 1];
    thread_t *thread_manager = malloc(sizeof(thread_t));
    druid_t *druid = build_a_new_druid(pot_size, nb_refills);
    arg_druid_t *arg = malloc(sizeof(arg_druid_t));

    arg->druid = druid;
    arg->nb_refills = nb_refills;
    arg->pot_size = pot_size;
    arg->thread_manager = thread_manager;
    pthread_mutex_init(&thread_manager->pot_mutex, NULL);
    sem_init(&semaphore, PTHREAD_PROCESS_SHARED, 1);
    sem_init(&thread_manager->call_druid, 0, 0);
    sem_init(&thread_manager->pot_refilled, 0, 0);
    sem_init(&thread_manager->druid_can_exit, 0, 0);
    thread_manager->nb_waiting = 0;
    thread_manager->active_villagers = nb_villagers;
    if (pthread_create(&threads[0], NULL, druid_thread, arg) != 0) {
        free(thread_manager);
        free(arg);
        free(druid);
        return false;
    }
    for (size_t i = 1; i <= nb_villagers; i++) {
        arg_vilager_t *args = malloc(sizeof(arg_vilager_t));
        args->id = nb_villagers - i;
        args->nb_fights = nb_fights;
        args->druid = druid;
        args->thread_manager = thread_manager;
        if (pthread_create(&threads[i], NULL,villager_thread, args) != 0) {
            free(thread_manager);
            free(arg);
            free(druid);
            return false;
        }
    }
    for (size_t i = 0; i <= nb_villagers; i++)
        pthread_join(threads[i], NULL);
    sem_destroy(&semaphore);
    sem_destroy(&thread_manager->call_druid);
    sem_destroy(&thread_manager->pot_refilled);
    sem_destroy(&thread_manager->druid_can_exit);
    pthread_mutex_destroy(&thread_manager->pot_mutex);
    free(druid);
    free(thread_manager);
    return true;
}

int main(int argc, char **argv)
{
    unsigned int nb_villagers = 0;
    unsigned int pot_size = 0;
    unsigned int nb_fights = 0;
    unsigned int nb_refills = 0;

    if (argc != 5) {
        helper();
        return 84;
    }
    if (parse_values(&nb_villagers, &pot_size, &nb_fights, &nb_refills, argv) == false)
        return 84;
    if (run_game(nb_villagers, pot_size, nb_fights, nb_refills) == false)
        return 84;
    return 0;
}