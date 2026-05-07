/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** villager
*/

#include "panoramix.h"
#include <unistd.h>

void clear_villager(villager_t *villager)
{
    if (!villager)
        return;
    printf("Villager %d: I'm going to sleep now.\n", villager->id);
    free(villager);
    villager = NULL;
}

villager_t *build_a_new_villager(size_t nb_fights, size_t id)
{
    villager_t *villager = malloc(sizeof(villager_t));

    if (!nb_fights || !id)
        return NULL;
    villager->id = id;
    villager->nb_fights = nb_fights;
    villager->have_drink = false;
    printf("Villager %d: Going into battle!\n", villager->id);
    return villager;
}

bool try_to_drink(villager_t *villager, druid_t *druid)
{
    if (druid->pot == 0) {
        printf("Villager %d: Hey Pano wake up! We need more potion.\n", villager->id);
        return villager->have_drink;
    }
    villager->have_drink = true;
    printf("Villager %d: I need a drink... I see %d servings left.\n", villager->id, druid->pot);
    druid->pot--;
    druid->is_called = false;
    return villager->have_drink;
}

size_t villager_fight(villager_t *villager)
{
    if (!villager->have_drink)
        return  villager->nb_fights;
    villager->nb_fights--;
    printf("Villager %d: Take that roman scum! Only %d left.\n", villager->id, villager->nb_fights);
    villager->have_drink = false;
    return villager->nb_fights;
}