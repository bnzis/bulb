/* ====================================================================== *\
 * This file provides a method to count the number of digits of a decimal *
 * numeral, from char to unsigned long long sizes.                        *
 *                                                                        *
 * Written by Nomid Lomysz (TheLonelyAdventurer)                          *
\* ====================================================================== */

#include <stdio.h>

#include "digits_int.h"

/**
 * Count Digits of Decimal
 *
 * Algorithm:
 * Divide by 10 the "decimal" until its result goes below 1.
 * The digits number is the count of divisions made.
 *
 * Why "unsigned char":
 * UCHAR_MAX is defined as 255 in <limits.h>, which is surely enough to
 * even handle "unsigned long long" values.
 *
 * Why "void*":
 * So we can handle any decimal numeral.
 *
 * Why "DECTYPE":
 * Because of the "void*".
 */
unsigned char countd_d(void* decimal, enum DECTYPE type)
{
    unsigned char i = 1; /* The first division is done in the switch */

    float div = 0.0f; /* The temp result of the divisions */
    
    switch(type) /* The cast is only done once */
    {
    case DECTYPE_CHR:
        div = (float) (*((char*) decimal) / 10.0f);

        break;
    case DECTYPE_UCHR:
        div = (float) (*((unsigned char*) decimal) / 10.0f);

        break;

    case DECTYPE_SRT:
        div = (float) (*((short*) decimal) / 10.0f);

        break;
    case DECTYPE_INT:
        div = (float) (*((int*) decimal) / 10.0f);

        break;
    case DECTYPE_LNG:
        div = (float) (*((long*) decimal) / 10.0f);

        break;
    case DECTYPE_LNGLNG:
        div = (float) (*((long long*) decimal) / 10.0f);

        break;

    case DECTYPE_USRT:
        div = (float) (*((unsigned short*) decimal) / 10.0f);

        break;
    case DECTYPE_UINT:
        div = (float) (*((unsigned int*) decimal) / 10.0f);

        break;
    case DECTYPE_ULNG:
        div = (float) (*((unsigned long*) decimal) / 10.0f);

        break;
    case DECTYPE_ULNGLNG:
        div = (float) (*((unsigned long long*) decimal) / 10.0f);

        break;

    default: /* This is not supposed to happen */
        fprintf(stderr, "Error: countd_d: Unknown decimal type.");

        break;
    }

    for( ; ; i++)
        if(div < 1)
            break;
        else
            div /= 10.0f;

    return i;
}

