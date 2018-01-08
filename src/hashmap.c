/*
 *  Bulb - the Lisp Interpreter
 *  Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
 *  Copyright (C) 2012-2016, Yann Collet (xxhash)
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

unsigned long long hash(const void* buffer, int length)
{
    unsigned long long const seed = 0;  
    unsigned long long const hash = XXH64(buffer, length, seed);
    return hash;
}

void put(hashmap_t *map, char *key, obj_t *obj)
{
    unsigned index = hash(key, strlen(key));
    index %= HMAP_ROWS;
    obj_t *pair = malloc(sizeof(obj_t));
    pair->type = CONS;
    pair->data.cons.car = obj;
    map->data[index] = pair;
}

obj_t *get(hashmap_t *map, char *key)
{
    unsigned index = hash(key, strlen(key));
    index %= HMAP_ROWS;
    return map->data[index]->data.cons.car;
}