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
#ifndef BULB_UTIL_H
#define BULB_UTIL_H
#include <stdio.h>
#include "types.h"
#include "parser.h"

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

#endif /* UTIL_H */
