/**
 * @file steg-decode.c
 * @author Jan Škrabal (xskrab12)-FIT
 * @brief IJC-DU1 B enkodování tajné zprávy
 * @date 2022-02-22
 *  
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "error.h"
#include "ppm.h"
#include "bitset.h"
#include "eratosthenes.h"

#define PRIME_START 29


int main(int argc, char const *argv[]) {
    //! kontrola správného počtu argumentů
    if (argc != 2) error_exit("stag-decode: Chybný počet argumentů! %d argument/y nejsou 2\n",argc);
    
    //!##########################  ---- Zde upravit zprávu k zakováví ---- 
    //const char zprava[] = "START:\nPokud čteš tenhle text,\ndostal jsi se daleko\n\0\0";
    const char zprava[] = "START:\nPokud čteš tenhle text,\ndostal jsi se do takové fáze,\nže Ti nezbývá nic jeného než,\nodevzdat tento úkol.\nDoufám v plný počet bodů, nebo alespoň v co nejméně chyb.\nMake by BOT(Honza) :)\0text kteý nesmí nikdo vidět!\0";
    //!##########################

    int encode_zpava[(strlen(zprava)*__CHAR_BIT__)];
    size_t count = 0;

    //printf("delka: %ld\n",strlen(zprava));
    int tmp[__CHAR_BIT__] = {0,};
    for (size_t z = 0; z < strlen(zprava); z++) {
        for (size_t i = 0; i < __CHAR_BIT__; i++) {
            tmp[i] = zprava[z] & (1 << i);
            //printf("%d ",tmp[i]>0);
        }
        if (zprava[z]=='\0'){
            //printf("\nnašel \\ 0\n");
        }
        
        //printf("--%ld-- ",z);
        for (size_t r = 0; r < __CHAR_BIT__; r++) {
            encode_zpava[count] = (tmp[r] > 0);
            count++;
        }
    }
    /*
    for (size_t nul = 0; nul < __CHAR_BIT__; nul++) {
        encode_zpava[count] = 0;
        count++;
    }*/

    struct ppm *obrazek;                // vytvoření struktury pro obrázek
    obrazek = ppm_read(argv[1]);
    if (obrazek == NULL) error_exit("stag-encode: ppm_read vrátil chybu\n",0);

    bitset_index_t velikost = 3*(obrazek->xsize)*(obrazek->ysize); // velikost obrazových dat v bitech
    bitset_alloc(primes,velikost);
    Eratosthenes(primes);               // najití všech prvočísel od 0 do velikosti dat obrázku

    //printf("\n--odělovač--\n");
    count = 0;
    size_t en;
    for (en = PRIME_START; en < velikost; en++) {
        if ((bitset_getbit(primes,en)) == 0) {
            //printf("add 000 %d\n",count);
            //int encode = (zprava[(count/8)] << (count%8));
            //printf("%c",zprava[(count/8)]);
            obrazek->data[en] = encode_zpava[count];

            count++;
        }
        if (count == (strlen(zprava)*__CHAR_BIT__)) {
            //i = velikost;
            break;
        }
    }
    //printf("%lu skončil na ",en);
    for (size_t en_end = en +(__CHAR_BIT__*strlen(zprava)); en < en_end; en++) {
        if ((bitset_getbit(primes,en)) == 0) {
            obrazek->data[en] = 0;
        }
    }
    //printf("%lu skončil na ",en);
    //! uspešně zakodováno
    //printf("vel:%dx%d\n",obrazek->xsize,obrazek->ysize);


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
                //printf("\njump\n");
                i = velikost;
                lsb_count = -1;
            }
            znak = 0;
        }
    }

    if (ppm_write(argv[1],obrazek)) error_exit("stag-encode: ppm_write vrátil chybu\n",0);
    printf("\nZpráva zapsána\n");                       // na konec zprávy přidá nový řádek

    ppm_free(obrazek);                  // uvolnění struktury obrázku
    bitset_free(primes);                // uvolnění pole prvočísel

    return 0;
}
