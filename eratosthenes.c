/**
 * @file eratosthenes.c
 * @author Jan Škrabal (xskrab12)-FIT
 * @brief IJC-DU1 B Eratostenovo síto
 * @date 2022-02-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "eratosthenes.h"

void Eratosthenes(bitset_t *pole) {
    bitset_setbit(pole,0,1);    //0 není prvočíslo
    bitset_setbit(pole,1,1);    //1 není prvočíslo
    for (bitset_index_t i = 2; i*i < (bitset_size(pole)); i++) {
        if ((bitset_getbit(pole,i)) == 0) {                                     //pokud je číslo 
            for (bitset_index_t j = i*i ; j < (bitset_size(pole)); j +=i) {    //škrtá všechny násobky daného prvočíla
                bitset_setbit(pole,j,1);    
            }
        }
    }
}
//! čas 6.2s

/*

void eratosthenes(bitset_t *pole) {
    bitset_setbit(pole,0,1);    //0 není prvočíslo
    bitset_setbit(pole,1,1);    //1 není prvočíslo
    for (bitset_index_t i = 2; i <=  (bitset_index_t)sqrt(bitset_size(pole)); i++) {
        //printf("era..i: %lu >> %u\n",i,bitset_getbit(pole,i));
        if ((bitset_getbit(pole,i)) == 0) {

            //printf("škrt \n");
            for (bitset_index_t j = 2; j <= (bitset_size(pole))/i; j++) {
                //if ((bitset_getbit(pole,i)) == 0) {
                    bitset_setbit(pole,(i*j),1);
                    //printf("i:%lu  ,j:%lu  ",i,j);
                //}
            }
        }
    }
}
//! čas cca 16,1s
*/