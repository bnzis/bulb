/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
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
    return exp[i] >= '0' && exp[i] <= '9' && dots <= 1; 
}

bool bulbIsHex(char *exp, unsigned len)
{
    unsigned i = 0, x = 0;
    bool cond;
    if (exp[0] != '0') return false;
    while (i < len && (cond = ((exp[i] >= '0' && exp[i] <= '9') || 
            (exp[i] >= 'a' && exp[i] <= 'f') || (exp[i] >= 'A' && exp[i] <= 'F')
            || (exp[i] == 'x' && x < 1))))  {
        if (exp[i] == 'x') x++;
        i++;
    }
    return cond && x == 1;
}

bulbObj *bulbGenAtom(char *exp, unsigned len)
{
    bulbObj *o = bulbNewObj();
    if (bulbIsInt(exp, len)) {
        o->data = (bulbInt*) malloc(sizeof(bulbInt)); 
        *((bulbInt*) o->data) = atol(exp);
        o->type = BULB_INT_TAG;
    } else if (bulbIsFloat(exp, len)) {
        o->data = (bulbFloat*) malloc(sizeof(bulbFloat));
        *((bulbFloat*) o->data) = atof(exp);
        o->type = BULB_FLOAT_TAG;
    } else if (bulbIsHex(exp, len)) {
        o->data = (bulbInt*) malloc(sizeof(bulbInt)); 
        *((bulbInt*) o->data) = strtol(exp, (char**) exp + len, 16);
        o->type = BULB_INT_TAG;
    } else if (exp[0] == '\"') {
        o->data = (bulbString*) malloc(sizeof(bulbString));
        ((bulbString*) o->data)->data = (char*) malloc(sizeof(char) * (len - 1));
        ((bulbString*) o->data)->len = len - 2;
        memcpy(((bulbString*) o->data)->data, exp + 1, len - 2);
        ((bulbString*) o->data)->data[len - 2] = '\0';
        o->type = BULB_STRING_TAG;
    } else if (exp[0] == '#' && (exp[1] == 't' || exp[1] == 'f')) {
        o = (exp[1] == 't')? bulbTrue : bulbFalse;
    } else if ((exp[0] >= 'a' && exp[0] <= 'z') || (exp[0] >= 'A' && exp[0] <= 'B') ||
                exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/' ||
                exp[0] == '=' || exp[0] == '>' || exp[0] == '<') {
        o->data = (bulbSymbol*) malloc(sizeof(bulbSymbol));
        ((bulbSymbol*) o->data)->data = (char*) malloc(sizeof(char) * len + 1);
        ((bulbSymbol*) o->data)->len = len;
        memcpy(((bulbSymbol*) o->data)->data, exp, len);
        ((bulbSymbol*) o->data)->data[len] = '\0';
        o->type = BULB_SYMBOL_TAG;
    } else {
        printf("Exception: %s: Identifiers must start with a letter.\n", exp);
        pthread_exit(NULL);
    }
    return o;
}

unsigned bulbLex(char *exp, unsigned len, unsigned *offset, bulbObj **out)
{
    if (*offset >= len) { 
        (*out) = bulbNil;
        return BULB_TOK_NIL;
    }
    exp = exp + *offset;
    len -= (*offset);
    unsigned aclen = 16, i = 0;
    char first, prev = 0;
    char *acc = (char*) malloc(aclen);
    bool str = false, comm = false;
    do {
        first = exp[0];
        exp++;
        (*offset)++;
        len--;
        if (first == '\n') {
            comm = false;
            if (str) {
                if (i >= aclen) {
                    aclen += aclen / 2;
                    acc = (char*) realloc(acc, aclen);
                }
                acc[i] = first;
                i++;
                prev = first;
            }
        } else if (!str && first == ';') comm = true;
        else if (!comm && !str && (i == 0 || len == 1) && first == '(') 
            return BULB_TOK_OPEN_BLOCK;
        else if (!comm && !str && i == 0  && first == ')')
            return BULB_TOK_CLOSE_BLOCK;
        else if (!str && !comm && (first == '(' || first == ')')) {
            (*offset)--;
            break;
        } else if (((first != ' ' && first != '\t' && first != '\\') || str) && !comm
            && prev != '\\') {
            if (first == '\"') str = !str;
            if (i >= aclen) {
                aclen += aclen / 2;
                acc = (char*) realloc(acc, aclen);
            }
            acc[i] = first;
            i++;
            prev = first;
        } else if (first == '\\' && prev == '\\') {
            if (i >= aclen) {
                aclen += aclen / 2;
                acc = (char*) realloc(acc, aclen);
            }
            acc[--i] = first;
            i++;
            prev = 0;
        } else if (prev == '\\') {
            if (i >= aclen) {
                aclen += aclen / 2;
                acc = (char*) realloc(acc, aclen);
            }
            if (first == 'n') acc[--i] = '\n';
            else if (first == 't') acc[--i] = '\t';
            else if (first == 'v') acc[--i] = '\v';
            else if (first == 'f') acc[--i] = '\f';
            else if (first == '\"') acc[--i] = '\"';
            else acc[i] = first;
            i++;
            prev = 0;
        } 
    } while (len > 0 && ((first != ' ' && first != '\t') || i == 0 || comm || str));
    if (str) bulb_err_missing_close_string();
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
    bulbObj *tree = bulbNewObj(), *front = tree, *tmp = bulbNewObj();
    unsigned ttype = bulbLex(exp, len, offset, &tmp); 
    while (ttype != BULB_TOK_CLOSE_BLOCK && ttype != BULB_TOK_NIL) {
        front->type = BULB_CONS_TAG;
        front->data = (bulbCons*) malloc(sizeof(bulbCons));
        if (ttype == BULB_TOK_OPEN_BLOCK)
            ((bulbCons*) front->data)->car = bulbGenAst(exp, len, offset, true);
        else    
            ((bulbCons*) front->data)->car = tmp;
        tmp = bulbNewObj();
        ttype = bulbLex(exp, len, offset, &tmp);
        if (ttype != BULB_TOK_NIL && ttype != BULB_TOK_CLOSE_BLOCK) {
            ((bulbCons*) front->data)->cdr = bulbNewObj();
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
    pthread_exit(NULL);
}

void bulb_err_missing_open_block()
{
    printf("Exception: missing \'(\'.\n");
    pthread_exit(NULL);
}

void bulb_err_missing_close_string()
{
    printf("Exception: missing \'\"\'.\n");
    pthread_exit(NULL);
}
