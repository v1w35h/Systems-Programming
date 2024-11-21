#include <stdio.h>

void print_unsigned(void)
{
    unsigned short myshort = 1334;
    unsigned int myint = 21934;
    unsigned long mylong = 77771899293UL;
    printf("%hu %u %lu\n", myshort, myint, mylong);
}

void print_signed(void)
{
    short myshort = -1334;
    int myint = -21934;
    long mylong = -77771899293L;
    printf("%hd %d %ld\n", myshort, myint, mylong);
}

void print_float(void)
{
    float myfloat = -12.775;
    double mydouble = -4414.229;
    // For printf (only), %f is used for 64-bit doubles, and printf
    // will promote myfloat to a double temporarily as if it is typecast.
    // We generally use %lf even in printf, the l just does nothing, but it
    // makes it so we don't have to remember two specifiers--one for printf
    // and another for scanf.
    printf("%.2f %.2lf\n", myfloat, mydouble);
    // The main point of %f is to print in "fixed" notation, whereas %e
    // is used for scientific notation and %a is used for hexadecimal notation.
    printf("%.2e %.2le %.2a %.2la\n", myfloat, mydouble, myfloat, mydouble);
}

int main(int argc, char *argv[])
{
    printf("Unsigned tests:\n");
    print_unsigned();
    printf("Signed tests:\n");
    print_signed();
    printf("Float tests\n");
    print_float();
}
