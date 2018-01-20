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

bool is_atom(obj_t *obj)
{
    return obj->type != CONS;
}

void print_atom(obj_t *obj)
{
    if (obj->type == INT) printf("%i", obj->data.integer);
    else if (obj->type == FLOAT) printf("%f", obj->data.floating);
    else if (obj->type == SYMBOL) printf("%s", obj->data.symbol.buff);
    else if (obj->type == STRING) printf("\"%s\"", obj->data.string.buff);
    else if (obj->type == BOOL) printf((obj->data.boolean)? "#t" : "#f");
    else if (obj->type == PROCEDURE) printf("#<PROCEDURE>");
    else if (obj->type == PRIMITIVE) printf("#<PRIMITIVE>");
}

void print_ast(obj_t *tree)
{
    if (is_atom(tree)) print_atom(tree);
    else if (car(tree) != NULL) {
            if (is_atom(car(tree))) print_atom(car(tree));
            else if(car(tree)->type == CONS) {
                printf("(");
                print_ast(car(tree));
                if (cadr(tree) != NULL && cadr(tree)->type != NIL) printf(") ");
                else printf(")");
                print_ast(cdr(tree));
                return;
            }
            if (cdr(tree)->type != NIL) printf(" ");
            print_ast(cdr(tree));
        }
}

obj_t *car(obj_t *list)
{
    return list->data.cons.car;
}

void set_car(obj_t *list, obj_t *val)
{
    list->data.cons.car = val;
}

obj_t *cdr(obj_t *list)
{
    return list->data.cons.cdr;
}

void set_cdr(obj_t *list, obj_t *val)
{
    list->data.cons.cdr = val;
}

obj_t *caar(obj_t *list)
{
    return list->data.cons.car->data.cons.car;
}

void set_caar(obj_t *list, obj_t *val)
{
    list->data.cons.car->data.cons.car = val;
}

obj_t *caadr(obj_t *list)
{
    return list->data.cons.cdr->data.cons.car->data.cons.car;
}

obj_t *cadr(obj_t *list)
{
    return list->data.cons.cdr->data.cons.car;
}

obj_t *caddr(obj_t *list)
{
    return list->data.cons.cdr->data.cons.cdr->data.cons.car;
}

void set_caddr(obj_t *list, obj_t *obj)
{
    list->data.cons.cdr->data.cons.cdr->data.cons.car = obj;
}

obj_t *cadddr(obj_t *list)
{
    return list->data.cons.cdr->data.cons.cdr->data.cons.cdr->data.cons.car;
}

obj_t *cdar(obj_t *list)
{
    return list->data.cons.car->data.cons.cdr;
}

void set_cdar(obj_t *list, obj_t *val)
{
    list->data.cons.car->data.cons.cdr = val;
}

unsigned list_len(obj_t *list)
{
    unsigned len = 0;
    while (car(list) != NIL) {
        len++;
        list = cdr(list);
    }
    return len;
}

obj_t *list_get(obj_t *list, unsigned index)
{
    unsigned i = 0;
    while (car(list) != NIL && i < index)
    {
        i++;
        list = cdr(list);
    }
    return car(list);
}
