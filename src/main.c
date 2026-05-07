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