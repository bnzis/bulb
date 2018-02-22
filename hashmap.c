/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
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
    if (map->data[index] == NULL) return NULL;
    char *readKey = bulbGetStringText(bulbGetCaar(map->data[index]));
    if (strcmp(readKey, key) == 0)
        return bulbGetCdar(map->data[index]);
    else {
        bulbObj *ptr = map->data[index];
        do {
            readKey = bulbGetStringText(bulbGetCaar(ptr));
            if (strcmp(readKey, key) != 0) ptr = bulbGetCdr(ptr);
            else return bulbGetCdar(ptr);
        } while (ptr != bulbNil);
        return NULL;
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
        return;
    }
    char *t = bulbGetStringText(bulbGetCaar(map->data[index]));
    if (strcmp(t, key) == 0) {
        bulbSetCdar(map->data[index], obj);
    } else {
        bulbObj **ptr = &((bulbCons*) map->data[index]->data)->cdr;
        while (*ptr != bulbNil) {
            t = bulbGetStringText(bulbGetCaar(*ptr));
            if (strcmp(t, key) == 0) {
                ((bulbCons*) ((bulbCons*) (*ptr)->data)->car->data)->cdr = obj;
                return;
            }
            ptr = &((bulbCons*) (*ptr)->data)->cdr;
        }
        data = bulbNewConsObj(bulbNewStringObj(key, strlen(key)), obj);
        pair = bulbNewConsObj(data, bulbNil);
        *ptr = pair;
    }
}

void bulbHashmapDelete(bulbHashmap *map, char *key)
{
    unsigned index = bulbXXHash(key, strlen(key));
    bulbObj **ptr;
    index %= HMAP_ROWS;
    if (map->data[index] == NULL) return;
    char *t = bulbGetStringText(bulbGetCaar(map->data[index]));
    if (strcmp(t, key) == 0) {
        ptr = &(map->data[index]);
        ((bulbCons*) ((bulbCons*) (*ptr)->data)->car->data)->cdr = bulbNil;
    } else {
        ptr = &((bulbCons*) map->data[index]->data)->cdr;
        while (*ptr != bulbNil) {
            t = bulbGetStringText(bulbGetCaar(*ptr));
            if (strcmp(t, key) == 0) {
                ((bulbCons*) ((bulbCons*) (*ptr)->data)->car->data)->cdr = bulbNil;
                return;
            }
        }
    }
}
