/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** main
*/

#include "panoramix.h"
#include <unistd.h>
#include <semaphore.h>

void helper(void)
{
    write(1, "USAGE: ./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>\n", 70);
}

int main(int argc, char **argv)
{
    sem_t semophore;

    if (argc == 1) {
        helper();
        return 84;
    }
    return 0;
}