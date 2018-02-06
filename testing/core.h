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
#ifndef BULB_CORE_H
#define BULB_CORE_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/* token types */
enum {
    BULB_TOK_NIL = 0, 
    BULB_TOK_OPEN_BLOCK, 
    BULB_TOK_CLOSE_BLOCK, 
    BULB_TOK_OTHER, 
};

typedef struct bulbObj_s {
    void (*type)(struct bulbObj_s *o);
    void *data;
} bulbObj;

/* base types */
typedef void (*bulbType)(bulbObj *o);
extern bulbType BULB_NIL, BULB_BOOL, BULB_INT, BULB_FLOAT, BULB_SYMBOL, 
        BULB_STRING, BULB_PROCEDURE, BULB_PRIMITIVE;
extern bulbType BULB_CONS;

void bulbPrintNil(bulbObj *o), bulbPrintBool(bulbObj *o), 
        bulbPrintInt(bulbObj *o), bulbPrintFloat(bulbObj *o), 
        bulbPrintSymbol(bulbObj *o), bulbPrintString(bulbObj *o), 
        bulbPrintProcedure(bulbObj *o), bulbPrintPrimitive(bulbObj *o), 
        bulbPrintCons(bulbObj *o);

extern bulbObj *bulbNil, *bulbTrue, *bulbFalse; 

/* HASHMAP SECTION */
#define HMAP_ROWS 64 

typedef struct {
    bulbObj **data;
} bulbHashmap;

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

/* Checks if the object is an atom type. */ 
bool isAtom(bulbObj *obj);

void bulbPrintAtom(bulbObj *obj);
void bulbPrintAtomDisplay(bulbObj *obj, bool display);

void bulbPrintAst(bulbObj *obj);
void bulbPrintAstDisplay(bulbObj *obj, bool display);

bulbCons *bulbMakeCons(bulbObj *obj);

bulbObj *bulbNewConsObj(bulbObj *car, bulbObj *cdr);

bulbObj *bulbNewStringObj(char *text, unsigned len);

char *bulbGetStringText(bulbObj *string);

/* Lists operations */
bulbObj *bulbGetCar(bulbObj *list);

void bulbSetCar(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCdr(bulbObj *list);

void bulbSetCdr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCaar(bulbObj *list);

void bulbSetCaar(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCaadr(bulbObj *list);

void bulbSetCaadr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCadr(bulbObj *list);

void bulbSetCadr(bulbObj *list, bulbObj *val);;

bulbObj *bulbGetCaddr(bulbObj *list);

void bulbSetCaddr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCddr(bulbObj *list);

void bulbSetCddr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCdar(bulbObj *list);

void bulbSetCdar(bulbObj *list, bulbObj* val);

unsigned bulbListLen(bulbObj *list);

bulbObj *bulbListGet(bulbObj *list, unsigned index);

void bulb_err_not_a_pair(bulbObj *obj);

void bulb_err_out_of_bounds(unsigned index, unsigned len);

#endif /* BULB_CORE_H */
