/*
 *  Bulb - the Lisp Interpreter
 *  Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
 *  Copyright (C) 2012-2016 Yann Collet (xxhash)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "hashmap.h"
#include <stdio.h>

unsigned long long hash(const void* buffer, int length)
{
    unsigned long long const seed = 0;  
    unsigned long long const hash = XXH64(buffer, length, seed);
    return hash;
}

void append(hashmap_t *map, char *key, obj_t *obj)
{
    unsigned len = strlen(key), index = hash(key, len);
    index %= HMAP_ROWS;
    obj_t *pair = malloc(sizeof(obj_t));
    pair->type = CONS;
    pair->cons.car = malloc(sizeof(obj_t));
    pair->cons.car->type = CONS;
    pair->cons.car->cons.car = malloc(sizeof(obj_t));
    pair->cons.car->cons.car->type = SYMBOL;
    pair->cons.car->cons.car->symbol.buff = key;
    pair->cons.car->cons.car->symbol.len = len;
    pair->cons.car->cons.cdr = obj;
    if (map->data[index] == NULL)
        map->data[index] = pair;
    else {
        obj_t **ptr = &map->data[index]->cons.cdr;
        while ((*ptr) != NULL)
            ptr = &((*ptr)->cons.cdr);
        (*ptr) = pair;
    }
}

obj_t *get(hashmap_t *map, char *key)
{
    unsigned index = hash(key, strlen(key));
    index %= HMAP_ROWS;
    if (map->data[index] == NULL) return NULL;
    char *t = map->data[index]->cons.car->cons.car->symbol.buff;
    if (strcmp(t, key) == 0)
        return map->data[index]->cons.car->cons.cdr;
    else {
        obj_t *ptr = map->data[index];
        while (strcmp(t, key) != 0) {  
            ptr = ptr->cons.cdr;
            if (ptr == NULL) break;
            t = car(car(ptr))->symbol.buff;
        }
        if (ptr == NULL) return NULL;
        return cdar(ptr);
    }
}

void set(hashmap_t *map, char *key, obj_t *obj)
{
    unsigned index = hash(key, strlen(key));
    index %= HMAP_ROWS;
    if (map->data[index] == NULL) append(map, key, obj);
    char *t = map->data[index]->cons.car->cons.car->symbol.buff;
    if (strcmp(t, key) == 0)
        map->data[index]->cons.car->cons.cdr = obj;
    else {
        obj_t **ptr = &map->data[index];
        do {
            ptr = &((*ptr)->cons.cdr);
            if (*ptr == NULL) break;
            t = car(car(*ptr))->symbol.buff;
        } while (strcmp(t, key) != 0);
        if (*ptr == NULL) {
            *ptr = malloc(sizeof(obj_t));
            (*ptr)->type = CONS;
            set_car(*ptr, malloc(sizeof(obj_t)));
            (*ptr)->cons.car->type = CONS;
            (*ptr)->cons.car->cons.car = malloc(sizeof(obj_t));
            (*ptr)->cons.car->cons.car->type = SYMBOL;
            (*ptr)->cons.car->cons.car->symbol.buff = key;
            (*ptr)->cons.car->cons.car->symbol.len = strlen(key);
            (*ptr)->cons.car->cons.cdr = obj;
            return;
        }
        set_cdar(*ptr, obj);
    }
}

void delete(hashmap_t *map, char *key)
{
    unsigned index = hash(key, strlen(key));
    index %= HMAP_ROWS;
    if (map->data[index] == NULL) return;
    char *t = map->data[index]->cons.car->cons.car->symbol.buff;
    if (strcmp(t, key) == 0) {
        map->data[index] = map->data[index]->cons.cdr;
    } else {
        obj_t **ptr = &map->data[index];
        do {
            ptr = &((*ptr)->cons.cdr);
            if (*ptr == NULL) break;
            t = (*ptr)->cons.car->cons.car->symbol.buff;
        } while (strcmp(t, key) != 0);
        if (*ptr == NULL) return;
        (*ptr) = (*ptr)->cons.cdr;
    }
}
