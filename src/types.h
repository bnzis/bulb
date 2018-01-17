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
#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>

/* base types */
enum {
    NIL, 
    SYMBOL, 
    STRING, 
    INT, 
    FLOAT, 
    BOOL, 
    CONS, 
    PROCEDURE,
    PRIMITIVE, 
};

/* token types */
enum {
    OPEN_BLOCK = 8, 
    CLOSE_BLOCK, 
    OTHER, 
};

struct obj;
typedef struct obj obj_t;

struct cons {
   obj_t *car;
   obj_t *cdr;
};

struct procedure {
    obj_t *args;
    obj_t *body;
    // env_t *env;
};

/* HASHMAP SECTION */
#define HMAP_ROWS 128 /* must be a power of 2. */

typedef struct hashmap {
    obj_t **data;
} hashmap_t;

/* ENVIRONMENT SECTION */
struct environment;
typedef struct env env_t;

typedef struct env {
    hashmap_t *local;
    env_t *upper_level;
} env_t;


typedef struct obj {
    short type;
    union obj_data {
        struct {
            char *buff;
            unsigned len;
        } symbol;
        struct {
            char *buff;
            unsigned len;
        } string;
        int integer;
        float floating;
        bool boolean;
        struct cons cons;
        struct procedure procedure;
        obj_t* (*primitive)(obj_t *args, env_t *env);
    } data;
} obj_t;

#endif
