/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#include "core.h"

unsigned char bulbLoadType(bulbType type)
{
    if (!bulbTypeTable) {
        bulbTypeTableCount = 0;
        bulbTypeTableSize = BULB_DEFAULT_TYPE_TABLE_SIZE;
        size_t newSize = sizeof(bulbType) * bulbTypeTableSize;
        bulbTypeTable = (bulbType *) malloc(newSize);
    }
    if (bulbTypeTableCount == bulbTypeTableSize - 1) {
        bulbTypeTableSize *= 2;
        size_t newSize = sizeof(bulbType) * bulbTypeTableSize;
        bulbTypeTable = (bulbType *) realloc(bulbTypeTable, newSize);
    }
    bulbTypeTable[++bulbTypeTableCount] = type;
    return bulbTypeTableCount;
}


bulbType BULB_NIL = {bulbNilToString, NULL},  
         BULB_BOOL = {bulbBoolToString, NULL}, 
         BULB_INT = {bulbIntToString, bulbFreeInt}, 
         BULB_FLOAT = {bulbFloatToString, bulbFreeFloat}, 
         BULB_SYMBOL = {bulbSymbolToString, bulbFreeSymbol}, 
         BULB_STRING = {bulbStringToString, bulbFreeString}, 
         BULB_PROCEDURE = {bulbProcedureToString, bulbFreeProcedure}, 
         BULB_PRIMITIVE = {bulbPrimitiveToString, NULL},
         BULB_CONS = {bulbConsToString, bulbFreeCons};

bulbObj bulbNilObj = {&bulbNilToString, 0};
bulbObj *bulbNil = &bulbNilObj;

bulbObj bulbTrueObj = {&bulbBoolToString,  0};
bulbObj *bulbTrue = &bulbTrueObj;

bulbObj bulbFalseObj = {&bulbBoolToString, 0};
bulbObj *bulbFalse = &bulbFalseObj;

char *bulbNilToString(bulbObj *o) 
{
    char *out = (char*) malloc(strlen("\b")); 
    strcpy(out, "\b");
    return out;
}

char *bulbBoolToString(bulbObj *o) 
{
    char *out = (char*) malloc(strlen("#t")); 
    strcpy(out, (o == bulbTrue) ? "#t" : "#f");
    return out;
} 

char *bulbIntToString(bulbObj *o) 
{
    /* checks the size before allocating the string */
    size_t size = snprintf(NULL, 0, "%ld", *((bulbInt *) o->data)) + 1;
    char *out = (char*) malloc(size);
    snprintf(out, size, "%ld", *((bulbInt *) o->data));
    return out;
} 

char *bulbFloatToString(bulbObj *o) 
{
    /* checks the size before allocating the string */
    size_t size = snprintf(NULL, 0, "%lf", *((bulbFloat *) o->data)) + 1;
    char *out = (char*) malloc(size);
    snprintf(out, size, "%lf", *((bulbFloat *) o->data));
    return out;
} 

char *bulbSymbolToString(bulbObj *o) 
{
    char *out = (char*) malloc(strlen((*((bulbSymbol *) o->data)).data));
    strcpy(out, (*((bulbSymbol *) o->data)).data);
    return out;
} 

char *bulbStringToString(bulbObj *o) 
{
    char *out = (char*) malloc(strlen((*((bulbSymbol *) o->data)).data));
    strcpy(out, (*((bulbSymbol *) o->data)).data);
    return out;
} 

char *bulbProcedureToString(bulbObj *o) 
{
    char *out = (char*) malloc(strlen("#<PROCEDURE>"));
    strcpy(out, "#<PROCEDURE>");
    return out;
}

char *bulbPrimitiveToString(bulbObj *o) 
{
    char *out = (char*) malloc(strlen("#<PRIMITIVE>")); 
    strcpy(out, "#<PRIMITIVE>");
    return out;
}

char *bulbConsToString(bulbObj *o) 
{
    return NULL;
}

void bulbFreeInt(bulbObj *o)
{
   free(o->data);     
} 

void bulbFreeFloat(bulbObj *o)
{
    free(o->data);
}

void bulbFreeSymbol(bulbObj *o)
{
    free(((bulbSymbol *) o->data)->data);
    free(o->data);
} 

void bulbFreeString(bulbObj *o)
{
    free(((bulbString *) o->data)->data);
    free(o->data);
}

void bulbFreeProcedure(bulbObj *o)
{
    free(bulbGetProcEnv(o)->local);
    free(bulbGetProcEnv(o));
    free(o->data);
} 

void bulbFreeCons(bulbObj *o)
{
   free(o->data); 
}

bool bulbIsAtom(bulbObj *obj)
{
    return obj->type != BULB_CONS_TAG;
}

void bulbPrintAtom(bulbObj *obj)
{
    bulbPrintAtomDisplay(obj, true);
}

void bulbPrintAtomDisplay(bulbObj *obj, bool display)
{
    if (obj->type == BULB_CONS_TAG) bulb_err_not_a_pair(obj);
    if ((obj->type) == 0) {
        printf("#<UNKNOWN>");
        return;
    }
    if (obj->type == BULB_STRING_TAG && display) {
        printf("\"");
        char *str = bulbTypeTable[obj->type].toString(obj);
        printf("%s", str);
        free(str);
        printf("\"");
    } else {
        char *str = bulbTypeTable[obj->type].toString(obj);
        printf("%s", str);
        free(str);
    }
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
        while (tree != bulbNil && tree->type == BULB_CONS_TAG) {
            if (bulbGetCar(tree)->type == BULB_CONS_TAG) { 
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
    if (tree->type == BULB_NIL_TAG) printf(" ");
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
    while (list->type == BULB_CONS_TAG)
        list = bulbGetCdr(list);
    return list == bulbNil;
}

unsigned bulbListLen(bulbObj *list)
{
    unsigned len = 0;
    while (list->type == BULB_CONS_TAG) {
        len++;
        list = bulbGetCdr(list);
    }
    return len;
}

bulbObj *bulbListGet(bulbObj *list, unsigned index)
{
    unsigned i = 0, len = bulbListLen(list);
    if (index > len) bulb_err_out_of_bounds(index, len);
    while (bulbGetCar(list)->type != BULB_NIL_TAG && i < index) {
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
