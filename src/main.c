/*
** EPITECH PROJECT, 2026
** G-CCP-400-STG-4-1-panoramix-16
** File description:
** main
*/

#include "panoramix.h"
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
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
    unsigned int **value_array = malloc(sizeof(int) * 4);
    for (size_t i = 2; argv[i] != NULL; i++, j++)
        value_array[j] = atoi(argv[i]);
    nb_villagers = value_array[0];
    pot_size = value_array[1];
    nb_fights = value_array[2];
    nb_refills = value_array[3];
    free_array(argv);
    free_array(value_array);
    return true;
}

int main(int argc, char **argv)
{
    sem_t semaphore;
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
    pthread_t threads[nb_villagers + 1];
    sem_init(&semaphore, PTHREAD_PROCESS_SHARED, 1);
    return 0;
}