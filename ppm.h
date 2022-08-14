/**
 * @file ppm.h
 * @author Jan Škrabal (xskrab12)-FIT
 * @brief IJC-DU1 B hlavičkový soubor funkce nad odrázkem ve formátu .ppm
 * @date 2022-02-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef IJC_PPM_LIB
#define IJC_PPM_LIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#define MAGIC_NUM "P6"
#define ROZSAH_BAREV 255
#define LIMIT 8000*8000*3


/**
 * @brief datová struktura orázku .ppm
 * 
 */
struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];  // RGB bajty, celkem 3*xsize*ysize
};

/**
 * @brief načte obsah PPM souboru do struktury (dynamické)
 * 
 * @param filename      název souboru s příponou např.: du1-obrazek.ppm
 * @return struct ppm*  vrací ukazatel na strukturu
 */
struct ppm *ppm_read(const char * filename);

/**
 * @brief zapíše obsah PPM struktury do souboru
 * 
 * @param filename      název souboru s příponou např.: du1-obrazek.ppm
 * @param p             ukazatel na strukturu kterou chceme zapsat
 * @return int          vrací 1-úspšch, 0-chyba
 */
int ppm_write(const char * filename, struct ppm *p);

/**
 * @brief uvolní paměť dynamicky alokovanou v ppm_read
 * 
 * @param p             ukazatel na strukturu
 */
void ppm_free(struct ppm *p);

#endif  //IJC_PPM_LIB