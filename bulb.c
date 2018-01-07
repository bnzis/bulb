/*
 *  Bulb - the Lisp Interpreter
 *  Copyright (C) 2018-2019 bnzis (bonzisoft@protonmail.com)
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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* base types */
enum {
    NIL, 
    SYMBOL, 
    STRING, 
    INT, 
    FLOAT, 
    BOOL, 
    CONS, 
    PROCEDURE, 
};

/* token types */
enum {
    OPEN_BLOCK = 8, 
    CLOSE_BLOCK, 
    OTHER, 
};

struct obj;
typedef struct obj obj_t;

struct cons {
   obj_t *car;
   obj_t *cdr;
};

struct procedure {
    obj_t *args;
    obj_t *body;
    // env_t *env;
};

typedef struct obj {
    short type;
    union obj_data {
        struct {
            char *buff;
            unsigned len;
        } symbol;
        struct {
            char *buff;
            unsigned len;
        } string;
        int integer;
        float floating;
        bool boolean;
        struct cons cons;
        struct procedure procedure;
    } data;
} obj_t;

bool is_int(char *exp, unsigned len)
{
    unsigned i = 0;
    char c = exp[0];
    bool f;
    while ((f = c >= '0' && c <= '9') && i < len - 1) {
        i++;
        c = exp[i];
    }
    return f;
}

bool is_float(char *exp, unsigned len)
{
    unsigned i = 0, dots = 0;
    while (i < len - 1 && ((exp[i] >= '0' && exp[i]) <= '9') ||
        exp[i] == '.' && dots < 2)  {
        if (exp[i] == '.') dots++;
        i++;
    }
    if (i > len)  i = len - 1;
    return exp[i] >= '0' && exp[i] <= '9' && dots <= 1; 
}

obj_t atom(char *exp, unsigned len)
{
    obj_t o;
    if (is_int(exp, len)) {
        int x = atoi(exp);
        o.type = INT;
        o.data.integer = x;
    } else if (is_float(exp, len)) {
        float x = atof(exp);
        o.type = FLOAT;
        o.data.floating = x;
    } else if (exp[0] == '\"') {
        o.type = STRING;
        o.data.string.len = len - 2;
        o.data.string.buff = malloc(sizeof(char) * (len - 2));
        memcpy(o.data.string.buff, exp + 1, len - 2);
    } else if (exp[0] == '#' && (exp[1] == 't' || exp[1] == 'f')) {
        o.type = BOOL;
        o.data.boolean = exp[1] == 't';
    } else {
        o.type = SYMBOL;
        o.data.symbol.len = len;
        o.data.symbol.buff = malloc(sizeof(char) * len);
        memcpy(o.data.symbol.buff, exp, len);
    }  
    return o;
}

unsigned get_token(char *exp, unsigned len, unsigned *offset, obj_t *out)
{
    if (*offset >= len) return NIL;
    unsigned aclen = 16, i = 0;
    char first, prev = 0;
    char *acc = malloc(aclen);
    bool str = false, comm = false;
    exp = exp + *offset;
    while (len > 0 && ((first != ' ' && first != '\t') || i == 0 || comm || str)) {
        first = exp[0];
        exp++;
        (*offset)++;
        len--;
        if (first == '\n') comm = false;
        else if (!str && first == ';') comm = true;
        else if (!comm && !str && i == 0 && first == '(')
            return OPEN_BLOCK;
        else if (!comm && !str && i == 0 && first == ')')
            return CLOSE_BLOCK;
        else if (!str && !comm && (first == '(' || first == ')'))
            break;
        else if (((first != ' ' && first != '\t' && first != '\\') || str) && !comm) {
            if (first == '\"' && prev != '\\') str = !str;
            if (i >= len) {
                aclen *= 2;
                acc = realloc(acc, aclen);
            }
            acc[i] = first;
            i++;
        } else if (first == '\\' && prev == '\\') {
            if (i >= len) {
                aclen *= 2;
                acc = realloc(acc, aclen);
            }
            acc[i] = first;
            i++;
        }
        prev = first;
    }
    acc = realloc(acc, i);
    out = realloc(out, sizeof(obj_t));
    *out = atom(acc, i);
    return OTHER;
}

obj_t *generate_ast(char *exp, unsigned len, unsigned *offset)
{
    obj_t *tree = malloc(sizeof(obj_t)), *front = tree;
    tree->type = CONS;
    unsigned ttype = get_token(exp, len, offset, front);
    while (ttype != NIL) {
        if (ttype == OPEN_BLOCK) {
            front->data.cons.car = generate_ast(exp, len, offset);
        } else if (ttype == CLOSE_BLOCK) {
            return tree;
        } else {
            obj_t *tmp = malloc(sizeof(obj_t));
            memcpy(tmp, front, sizeof(obj_t)); 
            front->type = CONS;
            front->data.cons.car = tmp;
        }
        front->data.cons.cdr = malloc(sizeof(obj_t));
        front = front->data.cons.cdr;
        front->type = CONS;
        ttype = get_token(exp, len, offset, front);
    }
    return tree;   
}

obj_t *parse(char *exp)
{
    unsigned i = 0, len = strlen(exp);
    obj_t *ast = generate_ast(exp, len, &i);
    return ast;
}

void print_ast(obj_t *tree) 
{
    if (tree) {
        if (tree->type == SYMBOL || tree->type == STRING) 
            printf("%s\n", tree->data.symbol.buff);
        else if (tree->type == INT)
            printf("%i\n", tree->data.integer);
        else if (tree->type == FLOAT)
            printf("%f\n", tree->data.floating);
        else if(tree->type == CONS) {
            print_ast(tree->data.cons.car);
            print_ast(tree->data.cons.cdr);
        }
    }
}

int main()
{
    char *program = "(define (square x) (* x x)) (square 2)";
    obj_t *tree = parse(program);
    print_ast(tree);
    return 0;
}