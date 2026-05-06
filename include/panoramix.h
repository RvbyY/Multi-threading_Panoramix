/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** panoramix
*/

#include <stddef.h>
#include <stdbool.h>

#ifndef INCLUDED_PANORAMIX_H
    #define INCLUDED_PANORAMIX_H

typedef struct villager_s {
    size_t id;
    size_t nb_fights;
    bool have_drink;
} villager_t;

typedef struct druid_s {
    bool is_called;
    size_t pot;
    size_t pot_size;
    size_t nb_refills;
} druid_t;

void clear_druid(druid_t *druid);
void clear_villager(villager_t *villager);
void helper(void);
druid_t *build_a_new_druid(size_t pot_size, size_t nb_refill);
size_t refill_pot(druid_t *druid);
villager_t *build_a_new_villager(size_t nb_fights, size_t id);
bool try_to_drink(villager_t *villager, druid_t *druid);
size_t villager_fight(villager_t *villager);

#endif
