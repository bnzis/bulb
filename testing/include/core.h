/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */ 
#ifndef BULB_CORE_H
#define BULB_CORE_H
#include <stdlib.h>
#include <stdio.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <stdint.h>
/* Description
   ===========
   This file contains the definition of data structures used by the interpreter 
   to represent objects and the used base types and some important functions 
   which are used almost everywhere.
   
   bulbObj
   -------
   The interpreter uses this structure to represent objects; as you can see, 
   thanks to the fact that data is a generic pointer, we can easily create new 
   types in C to be used by the interpreter. 
   C is a low-level programming language so to distinguish the various types we 
   need a tag, type, which is set to a function (pointer) 
   that prints the object. */
typedef struct bulbObj_s {
    void (*type)(struct bulbObj_s *o);
    void *data;
} bulbObj;

typedef void (*bulbType)(bulbObj *o);

/* Base types
   ----------
   The base types used: nil, bool, int, float, symbol (for the names of 
   variables and functions), string, procedure(s defined in bulb) and
   primitive(s defined in C). */
extern bulbType BULB_NIL, BULB_BOOL, BULB_INT, BULB_FLOAT, BULB_SYMBOL, 
        BULB_STRING, BULB_PROCEDURE, BULB_PRIMITIVE;
extern bulbType BULB_CONS;

void bulbPrintNil(bulbObj *o), bulbPrintBool(bulbObj *o), 
        bulbPrintInt(bulbObj *o), bulbPrintFloat(bulbObj *o), 
        bulbPrintSymbol(bulbObj *o), bulbPrintString(bulbObj *o), 
        bulbPrintProcedure(bulbObj *o), bulbPrintPrimitive(bulbObj *o), 
        bulbPrintCons(bulbObj *o);

/* Instead of allocating every single time an object for nil, true and false
   the interpreter re-use these: */
extern bulbObj *bulbNil, *bulbTrue, *bulbFalse; 

/* bulbInt and bulbFloat 
   --------------------- 
   For the moment the interpreter doesn't support bignums. */
typedef int64_t bulbInt;
typedef double bulbFloat;

/* bulbHashmap
   -----------
   This data structure is used in the representation of environments, (you can 
   find more details on hashmap.md): it contains an array of objects associated 
   to a key; to access the elements without searching them through out all the 
   array an hashing algorithm is used. */
#define HMAP_ROWS 32
typedef struct {
    bulbObj **data;
} bulbHashmap;

/* bulbEnv
   -------
   Environments are used as set of variables and procedures, so basically 
   everything declared in bulb is contained inside of an environment, (you can 
   find more details on environment.md):  
   `(def a 10) (def (func) (def b 20))`  
   The environment of func contains b, the upper 
   enviroment contains a and func. */
typedef struct bulbEnv_s {
    bulbHashmap *local;
    struct bulbEnv_s *upperEnv;
} bulbEnv;

/* bulbCons
   --------
   These structures are generic pairs, they are used a lot in Lisp, to 
   represent lists, trees and S-expressions.  
   `(cons 1 (cons 2 3))`  
   in this case, the car of the first cons contains 1, and the cdr contains a 
   pointer to the next cons, which contains 2 and 3.  
   `(def a 10)`  
   This expression can be represented as a linked list of cons:  
   `(cons "def" (cons "a" (cons 10 '())))`  
   More complex expressions with nesting are represented as trees:  
   `(def a (+ 1 9))`  
   `(cons "def" (cons "a" (cons (cons "+" ...) '()))`  
   So cons is the universal building block of data structures. */
typedef struct {
    bulbObj *car;
    bulbObj *cdr;
} bulbCons;

/* bulbProc
   --------
   Procedures are contains two lists, args and body, which contains the code
   of the procedure; for keeping track of the environment where the procedure
   were declared there is a pointer. */
typedef struct {
    bulbObj *args;
    bulbObj *body;
    bulbEnv *env;
} bulbProc;

/* bulbPrimitive
   -------------
   Primitives are procedures declared in C inside modules. */
typedef bulbObj* (*bulbPrimitive)(bulbObj *args, bulbEnv *env);

/* bulbSymbol and bulbString
   ------------------------- */
typedef struct {
    char *data;
    unsigned len;
} bulbSymbol;

typedef struct {
    char *data;
    unsigned len;
} bulbString;

/* bulbModule
   ----------
   A Module is collection of primitives, defined in C. */
typedef struct {
    bulbPrimitive *primitives;
    char **names;
} bulbModule;

/* isAtom
   ------
   Checks if the object is an atom. */
bool isAtom(bulbObj *obj);

/* Functions used for printing
   ---------------------------
   `bulbPrintAtomDisplay` and `bulbPrintAstDisplay` allow to choose if diplay 
   the double quotes when printing a string; `bulbPrintAtom` and `bulbPrintAst` 
   automatically do this. */
void bulbPrintAtom(bulbObj *obj);
void bulbPrintAtomDisplay(bulbObj *obj, bool display);
void bulbPrintAst(bulbObj *obj);
void bulbPrintAstDisplay(bulbObj *obj, bool display);

/* bulbMakeCons
   ------------
   Used as a shortcut for extracting by an object a bulbCons pointer. */
bulbCons *bulbMakeCons(bulbObj *obj);

/* bulbNewConsObj,  bulbNewStringObj ...
   -------------------------------------
   Creating a new object is a bit mechanical, I prefer to use these functions 
   for making code more clear and readable. */
bulbObj *bulbNewConsObj(bulbObj *car, bulbObj *cdr);

bulbObj *bulbNewStringObj(char *text, unsigned len);

bulbObj *bulbNewProcObj(bulbObj *args, bulbObj *body, bulbEnv *env);

bulbEnv *bulbNewEnv(bulbEnv *upperEnv);

/* Operations with procedures 
   -------------------------- */
bulbObj *bulbGetProcBody(bulbObj *proc);

bulbObj *bulbGetProcArgs(bulbObj *proc);

bulbEnv *bulbGetProcEnv(bulbObj *proc);

/* Operations with symbols
   ----------------------- */
char *bulbGetSymbolText(bulbObj *symbol);

/* Operations with strings
   ----------------------- */
char *bulbGetStringText(bulbObj *string);

/* Operations with cons
   --------------------- */
bulbObj *bulbGetCar(bulbObj *list);

void bulbSetCar(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCdr(bulbObj *list);

void bulbSetCdr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCaar(bulbObj *list);

void bulbSetCaar(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCaadr(bulbObj *list);

void bulbSetCaadr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCadr(bulbObj *list);

void bulbSetCadr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCaddr(bulbObj *list);

void bulbSetCaddr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCddr(bulbObj *list);

void bulbSetCddr(bulbObj *list, bulbObj *val);

bulbObj *bulbGetCdar(bulbObj *list);

void bulbSetCdar(bulbObj *list, bulbObj* val);

bool bulbIsAList(bulbObj *obj);

unsigned bulbListLen(bulbObj *list);

bulbObj *bulbListGet(bulbObj *list, unsigned index);

/* Exceptions
   ---------- */
void bulb_err_not_a_pair(bulbObj *obj);

void bulb_err_out_of_bounds(unsigned index, unsigned len);

#endif /* BULB_CORE_H */
