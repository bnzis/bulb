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

unsigned long long bulbXXHash(const void* buffer, int length)
{
    unsigned long long const hash = XXH64(buffer, length, 0);
    return hash;
}

void bulbHashmapAppend(bulbHashmap *map, char *key, bulbObj *obj)
{
    unsigned len = strlen(key), index = bulbXXHash(key, len);
    index %= HMAP_ROWS;
    bulbObj *data = bulbNewConsObj(bulbNewStringObj(key, strlen(key)), obj);
    bulbObj *pair = bulbNewConsObj(data, bulbNil);
    if (map->data[index] == NULL)
        map->data[index] = pair;
    else {
        bulbObj **ptr = &((bulbCons*) map->data[index]->data)->cdr;
        while (*ptr != bulbNil)
            ptr = &((bulbCons*) (*ptr)->data)->cdr;
        *ptr = pair;
    }
}

bulbObj *bulbHashmapGet(bulbHashmap *map, char *key)
{
    unsigned index = bulbXXHash(key, strlen(key));
    index %= HMAP_ROWS;
    if (map->data[index] == NULL) return bulbNil;
    char *t = bulbGetStringText(bulbGetCaar(map->data[index]));
    if (strcmp(t, key) == 0)
        return bulbGetCadr(map->data[index]);
    else {
        bulbObj *ptr = map->data[index];
        while (strcmp(t, key) != 0 && ptr != bulbNil) {
            ptr = bulbGetCdr(ptr);
            t = bulbGetStringText(bulbGetCaar(ptr));
        }
        if (ptr == bulbNil) return ptr;
        return bulbGetCdar(ptr);
    }
}

void bulbHashmapSet(bulbHashmap *map, char *key, bulbObj *obj)
{
    unsigned index = bulbXXHash(key, strlen(key));
    index %= HMAP_ROWS;
    bulbObj *data, *pair;
    if (map->data[index] == NULL) {
        data = bulbNewConsObj(bulbNewStringObj(key, strlen(key)), obj);
        pair = bulbNewConsObj(data, bulbNil);
        map->data[index] = pair;
    } 
    char *t = bulbGetStringText(bulbGetCaar(map->data[index]));
    if (strcmp(t, key) == 0)
        bulbSetCadr(map->data[index], obj);
    else {
        bulbObj **ptr = &((bulbCons*) map->data[index]->data)->cdr;
        do {
            ptr = &(((bulbCons*) (*ptr)->data)->cdr);
            t = bulbGetStringText(bulbGetCaar(map->data[index]));
        } while (strcmp(t, key) != 0 && *ptr != bulbNil);
        if (*ptr == bulbNil) {
            data = bulbNewConsObj(bulbNewStringObj(key, strlen(key)), obj);
            pair = bulbNewConsObj(data, bulbNil);
            *ptr = pair;
        } else
            ((bulbCons*) ((bulbCons*) (*ptr)->data)->car)->cdr = obj;
    }
}

/*
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
}*/
