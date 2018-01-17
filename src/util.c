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

obj_t *car(obj_t *list)
{
    return list->data.cons.car;
}

obj_t *cdr(obj_t *list)
{
    return list->data.cons.cdr;
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
