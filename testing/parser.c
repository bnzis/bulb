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
#include "parser.h"

bool bulbIsInt(char *exp, unsigned len)
{
    unsigned i = 0;
    char c = (exp[0] == '-')? exp[1] : exp[0];
    bool f;
    while ((f = c >= '0' && c <= '9') && i < len - 1) {
        i++;
        c = exp[i];
    }
    return f;
}

bool bulbIsFloat(char *exp, unsigned len)
{
    unsigned i, dots = 0;
    i = (exp[0] == '-')? 1 : 0;
    while (i < len - 1 && ((exp[i] >= '0' && exp[i] <= '9') 
            || (exp[i] == '.' && dots < 2)))  {
        if (exp[i] == '.') dots++;
        i++;
    }
    if (i > len)  i = len - 1;
    return exp[i] >= '0' && exp[i] <= '9' && dots <= 1; 
}

bulbObj *bulbGenAtom(char *exp, unsigned len)
{
    bulbObj *o = (bulbObj*) malloc(sizeof(bulbObj));
    if (bulbIsInt(exp, len)) {
        o->data = (bulbInt*) malloc(sizeof(bulbInt)); 
        *((bulbInt*) o->data) = atol(exp);
        o->type = BULB_INT;
    } else if (bulbIsFloat(exp, len)) {
        o->data = (bulbFloat*) malloc(sizeof(bulbFloat));
        *((bulbFloat*) o->data) = atof(exp);
        o->type = BULB_FLOAT;
    } else if (exp[0] == '\"') {
        o->data = (bulbString*) malloc(sizeof(bulbString));
        ((bulbString*) o->data)->data = (char*) malloc(sizeof(char) * (len - 1));
        ((bulbString*) o->data)->len = len - 2;
        memcpy(((bulbString*) o->data)->data, exp + 1, len - 2);
        ((bulbString*) o->data)->data[len - 2] = '\0';
        o->type = BULB_STRING;
    } else if (exp[0] == '#' && (exp[1] == 't' || exp[1] == 'f')) {
        o = (exp[1] == 't')? bulbTrue : bulbFalse;
    } else {
        o->data = (bulbSymbol*) malloc(sizeof(bulbSymbol));
        ((bulbSymbol*) o->data)->data = (char*) malloc(sizeof(char) * len + 1);
        ((bulbSymbol*) o->data)->len = len;
        memcpy(((bulbSymbol*) o->data)->data, exp, len);
        ((bulbSymbol*) o->data)->data[len] = '\0';
        o->type = BULB_SYMBOL;
    }
    return o;
}

unsigned bulbLex(char *exp, unsigned len, unsigned *offset, bulbObj **out)
{
    if (*offset >= len) { 
        (*out) = bulbNil;
        return BULB_TOK_NIL;
    }
    unsigned aclen = 16, i = 0;
    char first, prev = 0;
    char *acc = (char*) malloc(aclen);
    bool str = false, comm = false;
    exp = exp + *offset;
    len -= (*offset);
    while (len > 0 && ((first != ' ' && first != '\t') || i == 0 || comm || str)) {
        first = exp[0];
        exp++;
        (*offset)++;
        len--;
        if (first == '\n') comm = false;
        else if (!str && first == ';') comm = true;
        else if (!comm && !str && (i == 0 || len == 1) && first == '(') 
            return BULB_TOK_OPEN_BLOCK;
        else if (!comm && !str && i == 0  && first == ')')
            return BULB_TOK_CLOSE_BLOCK;
        else if (!str && !comm && (first == '(' || first == ')')) {
            (*offset)--;
            break;
        } else if (((first != ' ' && first != '\t' && first != '\\') || str) && !comm) {
            if (first == '\"' && prev != '\\') str = !str;
            if (i >= len) {
                aclen *= 2;
                acc = (char*) realloc(acc, aclen);
            }
            acc[i] = first;
            i++;
        } else if (first == '\\' && prev == '\\') {
            if (i >= len) {
                aclen *= 2;
                acc = (char*) realloc(acc, aclen);
            }
            acc[i] = first;
            i++;
        } 
        prev = first;
    } 
    if (i == 0 || acc[0] == '\0') {
       (*out) = bulbNil; 
       return BULB_TOK_NIL;
    }
    acc = (char*) realloc(acc, i);
    *out = bulbGenAtom(acc, i);
    return BULB_TOK_OTHER;
}

bulbObj *bulbGenAst(char *exp, unsigned len, unsigned *offset, bool open)
{
    bulbObj *tree = (bulbObj*) malloc(sizeof(bulbObj)), *front = tree, 
            *tmp = (bulbObj*) malloc(sizeof(bulbObj));
    unsigned ttype = bulbLex(exp, len, offset, &tmp); 
    while (ttype != BULB_TOK_CLOSE_BLOCK && ttype != BULB_TOK_NIL) {
        front->type = BULB_CONS;
        front->data = malloc(sizeof(bulbCons));
        if (ttype == BULB_TOK_OPEN_BLOCK)
            ((bulbCons*) front->data)->car = bulbGenAst(exp, len, offset, true);
        else    
            ((bulbCons*) front->data)->car = tmp;
        tmp = (bulbObj*) malloc(sizeof(bulbObj));
        ttype = bulbLex(exp, len, offset, &tmp);
        if (ttype != BULB_TOK_NIL && ttype != BULB_TOK_CLOSE_BLOCK) {
            ((bulbCons*) front->data)->cdr = (bulbObj*) malloc(sizeof(bulbObj));
            front = bulbGetCdr(front);
        } else {
            ((bulbCons*) front->data)->cdr = bulbNil;
        }
    }
    if (open && ttype != BULB_TOK_CLOSE_BLOCK) bulb_err_missing_close_block(); 
    else if (!open && ttype == BULB_TOK_CLOSE_BLOCK)
        bulb_err_missing_open_block();
    return tree;
}

bulbObj *bulbParse(char *exp)
{
    unsigned i = 0, len = strlen(exp);
    bulbObj *ast = bulbGenAst(exp, len, &i, false);
    return ast;
}


void bulb_err_missing_close_block()
{
    printf("Exception: missing \')\'.\n");
    exit(1);
}

void bulb_err_missing_open_block()
{
    printf("Exception: missing \'(\'.\n");
    exit(1);
}
