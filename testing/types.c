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
#include "types.h"

bulbType BULB_NIL = bulbPrintNil,  BULB_BOOL = bulbPrintBool, 
            BULB_INT = bulbPrintInt, BULB_FLOAT = bulbPrintFloat, 
            BULB_SYMBOL = bulbPrintSymbol, BULB_STRING = bulbPrintString, 
            BULB_PROCEDURE = bulbPrintProcedure, 
            BULB_PRIMITIVE = bulbPrintPrimitive;
bulbType BULB_CONS = bulbPrintCons;

void bulbPrintNil(bulbObj *o) 
{
}

void bulbPrintBool(bulbObj *o) 
{
    printf((*((bool *) o->data) == true) ? "#t" : "#f");
} 

void bulbPrintInt(bulbObj *o) 
{
    printf("%d", *((long *) o->data));
} 

void bulbPrintFloat(bulbObj *o) 
{
    printf("%f", *((double *) o->data));
} 

void bulbPrintSymbol(bulbObj *o) 
{
    printf((*((bulbSymbol *) o->data)).data);
} 

void bulbPrintString(bulbObj *o) 
{
    printf((*((bulbString *) o->data)).data);
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
    printf("#<PAIR>");
}
