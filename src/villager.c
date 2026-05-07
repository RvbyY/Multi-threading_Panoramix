/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** villager
*/

#include "../include/panoramix.h"
#include <unistd.h>

void clear_villager(villager_t *villager)
{
    if (!villager)
        return;
    printf("Villager %ld: I'm going to sleep now.\n", villager->id);
    free(villager);
    villager = NULL;
}

villager_t *build_a_new_villager(size_t nb_fights, size_t id)
{
    villager_t *villager = malloc(sizeof(villager_t));

    villager->id = id;
    villager->nb_fights = nb_fights;
    villager->have_drink = false;
    printf("Villager %ld: Going into battle!\n", villager->id);
    return villager;
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

bool try_to_drink(villager_t *villager, druid_t *druid, thread_t *thread_manager)
{
    printf("Villager %ld: I need a drink... I see %ld servings left.\n", villager->id, druid->pot);
    if (druid->pot == 0) {
        printf("Villager %ld: Hey Pano wake up! We need more potion.\n", villager->id);
        call_druid(druid, thread_manager);
        return villager->have_drink;
    }
    villager->have_drink = true;
    druid->pot--;
    druid->is_called = false;
    return villager->have_drink;
}

size_t villager_fight(villager_t *villager)
{
    if (!villager->have_drink)
        return  villager->nb_fights;
    villager->nb_fights--;
    printf("Villager %ld: Take that roman scum! Only %ld left.\n", villager->id, villager->nb_fights);
    villager->have_drink = false;
    return villager->nb_fights;
}