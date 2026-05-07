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
