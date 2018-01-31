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
// #include "util.h"

/* Acquires the AST from string */
bulbObj *bulbParse(char *exp);

/* Verifies if said string can be converted to an integer (doesn't accept 
 * decimals). */
bool bulbIsInt(char *exp, unsigned len);

/* Verifies if the string can be converted to a float (decimal part is 
 * needed). */
bool bulbIsFloat(char *exp, unsigned len);

/* Converts the string to an object. */
bulbObj *bulbGenAtom(char *exp, unsigned len);

/* Returns the type of the token contained in said string starting from its 
 * offset. This only works if the conversion is possible, therefore if it's not 
 * a parentesis, gets retured. */
unsigned bulbLex(char *exp, unsigned len, unsigned *offset, bulbObj *out);

/* Generates an AST starting from the provided offset. */
bulbObj *bulbGenAst(char *exp, unsigned len, unsigned *offset, bool open);

void bulb_err_missing_close_block();

void bulb_err_missing_open_block();

#endif
