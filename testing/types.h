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
#include <stdio.h>

/* token types */
enum {
    OPEN_BLOCK, 
    CLOSE_BLOCK, 
    OTHER, 
};

typedef struct bulbObj_s {
    void (*type)(struct bulbObj_s *o);
    void *data;
} bulbObj;

/* base types */
typedef void (*bulbType)(bulbObj *o);
extern bulbType BULB_NIL, BULB_BOOL, BULB_INT, BULB_FLOAT, BULB_SYMBOL, 
        BULB_STRING, BULB_PROCEDURE, BULB_PRIMITIVE;

void bulbPrintNil(bulbObj *o), bulbPrintBool(bulbObj *o), 
        bulbPrintInt(bulbObj *o), bulbPrintFloat(bulbObj *o), 
        bulbPrintSymbol(bulbObj *o), bulbPrintString(bulbObj *o), 
        bulbPrintProcedure(bulbObj *o), bulbPrintPrimitive(bulbObj *o);

/* HASHMAP SECTION */
#define HMAP_ROWS 128 /* must be a power of 2. */

typedef struct {
    bulbObj **data;
} bulbHashmap;

/* ENVIRONMENT SECTION */
typedef struct env_s {
    bulbHashmap *local;
    struct env_s *upper_level;
} bulbEnv;

typedef struct {
   bulbObj *car;
   bulbObj *cdr;
} bulbCons;

typedef struct {
    bulbObj *args;
    bulbObj *body;
    bulbEnv *env;
} bulbProc;

typedef struct {
    char *data;
    unsigned len;
} bulbSymbol;

typedef struct {
    char *data;
    unsigned len;
} bulbString;

#endif
