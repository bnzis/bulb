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
#include "util.h"

bool bulbIsAtom(bulbObj *obj)
{
    return obj->type != BULB_CONS;
}

void bulbPrintAtom(bulbObj *obj)
{
    if ((obj->type) == NULL) {
        printf("#<UNKNOWN>");
        return;
    }
    obj->type(obj);
}

void bulbPrintAst(bulbObj *tree)
{
    if (bulbIsAtom(tree)) {
        bulbPrintAtom(tree);
    } else if (bulbGetCar(tree) != NULL) {
        if (bulbGetCar(tree)->type == BULB_CONS) { 
            printf("(");
            bulbPrintAst(bulbGetCar(tree));
            printf(")");
        } else {
            bulbPrintAtom(bulbGetCar(tree));
            if (!bulbIsAtom(bulbGetCdr(tree))) printf(" ");
        }
        bulbPrintAst(bulbGetCdr(tree));
    }
}

bulbCons *makeCons(bulbObj *o)
{
    return (bulbCons*) o->data;
}

bulbObj *bulbGetCar(bulbObj *list)
{
    return makeCons(list)->car;
}

bulbObj *bulbGetCdr(bulbObj *list)
{
    return makeCons(list)->cdr;
}

bulbObj *bulbGetCadr(bulbObj *list)
{
    return bulbGetCar(bulbGetCdr(list));
}

/* WIP
void set_car(bulbObj *list, bulbObj *val)
{
    list->cons.car = val;
}

bulbObj *cdr(bulbObj *list)
{
    return list->cons.cdr;
}

void set_cdr(bulbObj *list, bulbObj *val)
{
    list->cons.cdr = val;
}

bulbObj *caar(bulbObj *list)
{
    return list->cons.car->cons.car;
}

void set_caar(bulbObj *list, bulbObj *val)
{
    list->cons.car->cons.car = val;
}

bulbObj *caadr(bulbObj *list)
{
    return list->cons.cdr->cons.car->cons.car;
}

bulbObj *cadr(bulbObj *list)
{
    return list->cons.cdr->cons.car;
}

bulbObj *caddr(bulbObj *list)
{
    return list->cons.cdr->cons.cdr->cons.car;
}

void set_caddr(bulbObj *list, bulbObj *obj)
{
    list->cons.cdr->cons.cdr->cons.car = obj;
}

bulbObj *cadddr(bulbObj *list)
{
    return list->cons.cdr->cons.cdr->cons.cdr->cons.car;
}

bulbObj *cdar(bulbObj *list)
{
    return list->cons.car->cons.cdr;
}

void set_cdar(bulbObj *list, bulbObj *val)
{
    list->cons.car->cons.cdr = val;
}

unsigned list_len(bulbObj *list)
{
    unsigned len = 0;
    while (car(list) != NIL) {
        len++;
        list = cdr(list);
    }
    return len;
}

bulbObj *list_get(bulbObj *list, unsigned index)
{
    unsigned i = 0;
    while (car(list) != NIL && i < index)
    {
        i++;
        list = cdr(list);
    }
    return car(list);
}*/
