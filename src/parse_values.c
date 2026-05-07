/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** parse_values
*/

#include "../include/panoramix.h"
#include <unistd.h>
#include <stdlib.h>

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