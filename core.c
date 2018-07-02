/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#include "core.h"

bulbType BULB_NIL = bulbPrintNil,  BULB_BOOL = bulbPrintBool, 
            BULB_INT = bulbPrintInt, BULB_FLOAT = bulbPrintFloat, 
            BULB_SYMBOL = bulbPrintSymbol, BULB_STRING = bulbPrintString, 
            BULB_PROCEDURE = bulbPrintProcedure, 
            BULB_PRIMITIVE = bulbPrintPrimitive;
bulbType BULB_CONS = bulbPrintCons;

bulbObj bulbNilObj = {&bulbPrintNil, NULL};
bulbObj *bulbNil = &bulbNilObj;

bulbObj bulbTrueObj = {&bulbPrintBool, &bulbNilObj};
bulbObj *bulbTrue = &bulbTrueObj;

bulbObj bulbFalseObj = {&bulbPrintBool, &bulbNilObj};
bulbObj *bulbFalse = &bulbFalseObj;

void bulbPrintNil(bulbObj *o) 
{
    printf("\b");
}

void bulbPrintBool(bulbObj *o) 
{
    printf((o == bulbTrue) ? "#t" : "#f");
} 

void bulbPrintInt(bulbObj *o) 
{
    printf("%ld", *((bulbInt *) o->data));
} 

void bulbPrintFloat(bulbObj *o) 
{
    printf("%lf", *((bulbFloat *) o->data));
} 

void bulbPrintSymbol(bulbObj *o) 
{
    printf("%s", (*((bulbSymbol *) o->data)).data);
} 

void bulbPrintString(bulbObj *o) 
{
    printf("%s", (*((bulbString *) o->data)).data);
} 

void bulbPrintProcedure(bulbObj *o) 
{
    printf("#<PROCEDURE>");
}

void bulbPrintPrimitive(bulbObj *o) 
{
    printf("#<PRIMITIVE>");
}

void bulbPrintCons(bulbObj *o) 
{
}

bool bulbIsAtom(bulbObj *obj)
{
    return obj->type != BULB_CONS;
}

void bulbPrintAtom(bulbObj *obj)
{
    bulbPrintAtomDisplay(obj, true);
}

void bulbPrintAtomDisplay(bulbObj *obj, bool display)
{
    if (obj->type == BULB_CONS) bulb_err_not_a_pair(obj);
    if ((obj->type) == NULL) {
        printf("#<UNKNOWN>");
        return;
    }
    if (obj->type == BULB_STRING && display) {
        printf("\"");
        obj->type(obj);
        printf("\"");
    } else obj->type(obj);
}


void bulbPrintAst(bulbObj *tree)
{
    if (!tree) return;
    bulbPrintAstDisplay(tree, true);
}

void bulbPrintAstDisplay(bulbObj *tree, bool display)
{
    if (!tree) return;
    bool list = false;
    if (bulbIsAtom(tree)) {
        bulbPrintAtomDisplay(tree, display);
        return;
    } else if (bulbGetCar(tree) != bulbNil) {
        list = bulbIsAList(tree);
        printf("(");
        while (tree != bulbNil && tree->type == BULB_CONS) {
            if (bulbGetCar(tree)->type == BULB_CONS) { 
                bulbPrintAstDisplay(bulbGetCar(tree), display);
                if (bulbGetCdr(tree) != bulbNil) printf(" ");
            } else {
                bulbPrintAtomDisplay(bulbGetCar(tree), display);
                if (bulbGetCdr(tree) != bulbNil) printf(" ");
            }
            tree = bulbGetCdr(tree);
        }
        if (!list) printf(". ");
    }
    if (tree->type == BULB_NIL) printf(" ");
    bulbPrintAtom(tree); 
    printf(")");
}

bulbCons *bulbMakeCons(bulbObj *obj)
{
    if (obj == NULL || bulbIsAtom(obj)) bulb_err_not_a_pair(obj);
    return (bulbCons*) obj->data;
}

bulbObj *bulbGetProcBody(bulbObj *proc)
{
    return ((bulbProc*) proc->data)->body;
}

bulbObj *bulbGetProcArgs(bulbObj *proc)
{
    return ((bulbProc*) proc->data)->args;
}

bulbEnv *bulbGetProcEnv(bulbObj *proc)
{
    return ((bulbProc*) proc->data)->env;
}

char *bulbGetSymbolText(bulbObj *symbol)
{
    return ((bulbSymbol*) symbol->data)->data;
}

char *bulbGetStringText(bulbObj *string)
{
    return ((bulbString*) string->data)->data;
}

bulbObj *bulbGetCar(bulbObj *list)
{
    return bulbMakeCons(list)->car;
}

void bulbSetCar(bulbObj *list, bulbObj *val)
{
    bulbMakeCons(list)->car = val;
}

bulbObj *bulbGetCdr(bulbObj *list)
{
    return bulbMakeCons(list)->cdr;
}

void bulbSetCdr(bulbObj *list, bulbObj *val)
{
    bulbMakeCons(list)->cdr = val;
}

bulbObj *bulbGetCaar(bulbObj *list)
{
    return bulbGetCar(bulbGetCar(list));
}

void bulbSetCaar(bulbObj *list, bulbObj *val)
{
    bulbSetCar(bulbGetCar(list), val);
}

bulbObj *bulbGetCddr(bulbObj *list)
{
    return bulbGetCdr(bulbGetCdr(list));
}

void bulbSetCddr(bulbObj *list, bulbObj *val)
{
    bulbSetCdr(bulbGetCdr(list), val);
}

bulbObj *bulbGetCadr(bulbObj *list)
{
    return bulbGetCar(bulbGetCdr(list));
}

void bulbSetCadr(bulbObj *list, bulbObj *val)
{
    bulbSetCar(bulbGetCdr(list), val);
}

bulbObj *bulbGetCdar(bulbObj *list)
{
    return bulbGetCdr(bulbGetCar(list));
}

void bulbSetCdar(bulbObj *list, bulbObj *val)
{
    bulbSetCdr(bulbGetCar(list), val);
}

bulbObj *bulbGetCaddr(bulbObj *list)
{
    return bulbGetCadr(bulbGetCdr(list));
}

bulbObj *bulbGetCaadr(bulbObj *list)
{
    return bulbGetCaar(bulbGetCdr(list));
}

bool bulbIsAList(bulbObj *list)
{
    while (list->type == BULB_CONS)
        list = bulbGetCdr(list);
    return list == bulbNil;
}

unsigned bulbListLen(bulbObj *list)
{
    unsigned len = 0;
    while (list->type == BULB_CONS) {
        len++;
        list = bulbGetCdr(list);
    }
    return len;
}

bulbObj *bulbListGet(bulbObj *list, unsigned index)
{
    unsigned i = 0, len = bulbListLen(list);
    if (index > len) bulb_err_out_of_bounds(index, len);
    while (bulbGetCar(list)->type != BULB_NIL && i < index) {
        i++;
        list = bulbGetCdr(list);
    }
    return bulbGetCar(list);
}

void bulb_err_not_a_pair(bulbObj *obj)
{
    printf("Exception: ");
    bulbPrintAst(obj);
    printf(" is not a pair.\n");
    pthread_exit(NULL);
}

void bulb_err_out_of_bounds(unsigned index, unsigned len)
{
    printf("Exception: index (%d) out of bounds (len: %d).\n", index, len);
    pthread_exit(NULL);
}
