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
#ifndef PARSER_H
#define PARSER_H
#include <stdlib.h>
#include <string.h>
#include "types.h"

obj_t *parse(char *exp);

bool is_int(char *exp, unsigned len);

bool is_float(char *exp, unsigned len);

obj_t atom(char *exp, unsigned len);

unsigned get_token(char *exp, unsigned len, unsigned *offset, obj_t *out);

obj_t *generate_ast(char *exp, unsigned len, unsigned *offset);

#endif
