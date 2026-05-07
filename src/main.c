/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** main
*/

#include "panoramix.h"
#include <unistd.h>
#include <stdlib.h>

void helper(void)
{
    write(1, "USAGE: ./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>\n", 70);
}

void free_array(void **array)
{
    for (size_t i = 0; array[i] != NULL; i++)
        free(array[i]);
    free(array);
}

bool parse_values(unsigned int *nb_villagers, unsigned int *pot_size,
    unsigned int *nb_fights, unsigned int *nb_refills, char **argv)
{
    size_t j = 0;

    for (size_t i = 2; argv[i] != NULL; i++)
        if (atoi(argv[i]) <= 0)
            return false;
    *nb_villagers = (unsigned int)atoi(argv[1]);
    *pot_size = (unsigned int)atoi(argv[2]);
    *nb_fights = (unsigned int)atoi(argv[3]);
    *nb_refills = (unsigned int)atoi(argv[4]);
    free_array(argv);
    return true;
}

void *druid_thread(void *args)
{
    arg_druid_t *arg = (arg_druid_t *)arg;

    while (arg->druid->nb_refills > 0) {
        sem_wait(&arg->thread_manager->call_druid);
        pthread_mutex_lock(&arg->thread_manager->pot_mutex);
        refill_pot(arg->druid);
        pthread_mutex_unlock(&arg->thread_manager->pot_mutex);
        for (size_t i = 0; i < arg->thread_manager->nb_waiting; i++)
            sem_post(&arg->thread_manager->pot_refilled);
        arg->thread_manager->nb_waiting = 0;
    }
    clear_druid(arg->druid);
    pthread_exit(EXIT_SUCCESS);
}

void *villager_thread(arg_vilager_t *args)
{
    villager_t *villager = build_a_new_villager(args->nb_fights, args->id);

    while (villager->nb_fights > 0) {
        pthread_mutex_lock(&args->thread_manager->pot_mutex);
        if (args->druid->pot == 0) {
            if (args->druid->is_called == false) {
                args->druid->is_called = true;
                sem_post(&args->thread_manager->call_druid);
            }
            pthread_mutex_unlock(&args->thread_manager->pot_mutex);
            sem_wait(&args->thread_manager->pot_refilled);
            pthread_mutex_lock(&args->thread_manager->pot_mutex);
        }
        try_to_drink(villager, args->druid);
        pthread_mutex_unlock(&args->thread_manager->pot_mutex);
        villager_fight(villager);
    }
    clear_villager(villager);
    pthread_exit(EXIT_SUCCESS);
}

bool run_game(unsigned int nb_villagers, unsigned int pot_size,
    unsigned int nb_fights, unsigned int nb_refills)
{
    sem_t semaphore;
    pthread_t threads[nb_villagers + 1];
    thread_t *thread_manager = malloc(sizeof(thread_t));
    druid_t *druid = build_a_new_druid(pot_size, nb_refills);
    arg_druid_t arg = {pot_size, nb_refills, thread_manager, druid};

    sem_init(&semaphore, PTHREAD_PROCESS_SHARED, 1);
    pthread_create(&threads[0], NULL, druid_thread, &arg);
    for (size_t i = 1; i <= nb_villagers; i++) {
        arg_vilager_t *args = malloc(sizeof(arg_vilager_t));
        args->id = nb_villagers - i + 1;
        args->nb_fights = nb_fights;
        args->druid = druid;
        args->thread_manager = thread_manager;
        pthread_mutex_init(&thread_manager->pot_mutex, NULL);
        sem_init(&thread_manager->call_druid, 0, 0);
        sem_init(&thread_manager->pot_refilled, 0, 0);
        pthread_create(&threads[i], NULL,
                villager_thread, &args);
    }
    for (size_t i = 0; i <= nb_villagers; i++)
        pthread_join(threads[i], NULL);
    sem_destroy(&semaphore);
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
    if (parse_values(&nb_villagers, &pot_size, &nb_fights, &nb_refills, argv) == false) {
        free_array(argv);
        return 84;
    }
    if (run_game(nb_villagers, pot_size, nb_fights, nb_refills) == false) {
        free(argv);
        return 84;
    }
    free(argv);
    return 0;
}