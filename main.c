#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lib/lib.h"
#include "big_int/big_int.h"

int main() {

    printf("\n");
    big_int *n1 = big_int_get("0001111000000000000000000000000000000000000000000000000000000000000000000000000");
    big_int_print(n1);
    big_int *n2 = big_int_get("011000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_int_print(n2);
    printf("gcd=");
    big_int *n3 = big_int_euclid_binary(n1,n2);
    big_int_print(n3);
    big_int_free(n1);
    big_int_free(n2);
    big_int_free(n3);

    return 0;
}

