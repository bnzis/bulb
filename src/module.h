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
#ifndef MODULES_H
#define MODULES_H
#include "bulb.h"
#include "modules/core.h"

void err_module_not_compiled(char *name)
{
    printf("Exception in load: module %s not compiled.\n", name);
    exit(1);
}

void err_module_not_found(char *name)
{
    printf("Exception in load: module %s not found.\n", name);
    exit(1);
}

obj_t *alloc_primitive()
{
    obj_t *primitive = malloc(sizeof(obj_t));
    primitive->type = PRIMITIVE;
    return primitive;
}

void load_module(env_t *env, obj_t *module)
{
    char *module_name;
    if (module->type == SYMBOL)
       module_name = module->data.symbol.buff;
    if (strcmp(module_name, "core") == 0) {
#ifdef MODULES_CORE_H
        obj_t *primitive = alloc_primitive();
        primitive->data.primitive = sum;
        env_set(env, "+", primitive);
#else
        err_module_not_compiled(module_name);
#endif
    } else {
        err_module_not_found(module_name);
    }
}

#endif
