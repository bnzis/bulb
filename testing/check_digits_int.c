/*
 * This program prints countd_d values, so we can check if it works correctly.
 */

#include <stdio.h>

#include "digits_int.h"

unsigned char countd_d(void* decimal, enum DECTYPE type);

int main(void)
{
    char c = 1;
    printf("Digits count of %hhd is %d\n", c, countd_d(&c, DECTYPE_CHR));
    unsigned char uc = 42;
    printf("Digits count of %hhu is %d\n", uc, countd_d(&uc, DECTYPE_UCHR));

    short s = 341;
    printf("Digits count of %hd is %d\n", s, countd_d(&s, DECTYPE_SRT));
    unsigned short us = 1314;
    printf("Digits count of %hu is %d\n", us, countd_d(&us, DECTYPE_USRT));

    int i = 32123;
    printf("Digits count of %d is %d\n", i, countd_d(&i, DECTYPE_INT));
    unsigned int ui = 938251;
    printf("Digits count of %u is %d\n", ui, countd_d(&ui, DECTYPE_UINT));

    long l = 4235639;
    printf("Digits count of %ld is %d\n", l, countd_d(&l, DECTYPE_LNG));
    unsigned long ul = 11132458;
    printf("Digits count of %lu is %d\n", ul, countd_d(&ul, DECTYPE_ULNG));

    long long ll = 623689109;
    printf("Digits count of %lld is %d\n" ll, countd_d(&ll, DECTYPE_LNGLNG));
    unsigned long long ull = 1234567890;
    printf("Digits count of %llu is %d\n", ull, countd_d(&ull, DECTYPE_ULNGLNG));

    return 0;
}

