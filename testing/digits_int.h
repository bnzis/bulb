/*
 * This file contains the enum definition for digits_int.c
 */

#ifndef H_DIGITS_INT
#define H_DIGITS_INT

enum DECTYPE
{
    DECTYPE_CHR,        /* char */
    DECTYPE_UCHR,       /* unsigned char */

    DECTYPE_SRT,        /* short */
    DECTYPE_INT,        /* int */
    DECTYPE_LNG,        /* long */
    DECTYPE_LNGLNG,     /* long long */

    DECTYPE_USRT,       /* unsigned short */
    DECTYPE_UINT,       /* unsigned int */
    DECTYPE_ULNG,      /* unsigned long */
    DECTYPE_ULNGLNG   /* unsigned long long */
} ;

#endif // H_DIGITS_INT

