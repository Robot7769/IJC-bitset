/**
 * @file steg-decode.c
 * @author Jan Škrabal (xskrab12)-FIT
 * @brief IJC-DU1 B dekodování tajné zprávy
 * @date 2022-02-22
 *  
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "ppm.h"
#include "bitset.h"
#include "eratosthenes.h"

#define PRIME_START 29


int main(int argc, char const *argv[]) {
    //! kontrola správného počtu argumentů
    if (argc != 2) error_exit("stag-decode: Chybný počet argumentů! %d argument/y nejsou 2\n",argc);

    struct ppm *obrazek;                // vytvoření struktury pro obrázek
    obrazek = ppm_read(argv[1]);
    if (obrazek == NULL) error_exit("stag-decode: ppm_read vrátil chybu\n",0);

    bitset_index_t velikost = 3*obrazek->xsize*obrazek->ysize; // velikost obrazových dat v bitech
    bitset_alloc(primes,velikost);
    Eratosthenes(primes);               // najití všech prvočísel od 0 do velikosti dat obrázku

    int znak = 0;
    int lsb_count = 0;

    for (size_t i = PRIME_START; i < velikost; i++) {
        if ((bitset_getbit(primes,i)) == 0) {
            int bit = (obrazek->data[i] & 1);

            znak ^= bit << lsb_count;
            lsb_count++;
        }
        if (lsb_count == 8 ) {
            printf("%c", znak);
            lsb_count = 0;
            if (znak == '\0') {
                i = velikost;
                lsb_count = -1;
            }
            znak = 0;
        }
    }
    printf("\n");                       // na konec zprávy přidá nový řádek

    ppm_free(obrazek);                  // uvolnění struktury obrázku
    bitset_free(primes);                // uvolnění pole prvočísel
        
    if (lsb_count != -1)
        error_exit("stag-decode: chybný formát, zpráva není korektně ukončena '\0'\n",0);
    
    return 0;
}
