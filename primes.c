/**
 * @file primes.c
 * @author Jan Škrabal (xskrab12)-FIT
 * @brief IJC-DU1 A prvočísla
 * @date 2022-02-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitset.h"
#include "eratosthenes.h"

#define N 300000000

int main(void) {
    clock_t start = clock();
    //printf("inline: %d", USE_INLINE);

    bitset_alloc(a, N);


    Eratosthenes(a);
    
    bitset_t last_ten[10]= {0,};
    
    int count = 10;
    for (bitset_t i = (bitset_size(a)); count >= 0 ; i--) {
        if ( (bitset_getbit(a, i)) == 0) {
            count--;
            last_ten[count] = i;                 //vkládání vždy posledních 10 prvočísel, pokud se nejde další je přepsáno to nejmenší
        }
    }

    for (;count < 9; count++){
        printf("%ld\n",last_ten[count]);
    }

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    bitset_free(a);
    //! nezapomenout odpoznámkovta free ^^^^^^
    return 0;
}


//! starý výpis cca 1.21s 
/*
bitset_t last_ten[10]= {0,};
int last = 0;
//printf("\n\n");
    for (bitset_t i = 0; i <= (bitset_size(a)); i++) {
        if ( (bitset_getbit(a, i)) == 0) {
            last_ten[last] = i;                 //vkládání vždy posledních 10 prvočísel, pokud se nejde další je přepsáno to nejmenší
            if (last < 9) last++;
            else last = 0;
            //printf("%d\n", last);
        }
    }
    int end = last;
    do {
        printf("%ld\n",last_ten[last]);
        if (last < 9) last++;
        else last = 0;
    } while (last != end);
    
*/