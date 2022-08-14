/**
 * @file bitset.h
 * @author Jan Škrabal (xskrab12)-FIT
 * @brief IJC-DU1 A hlavičkový soubor makra bitset
 * @date 2022-02-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef IJC_BITSET_LIB
#define IJC_BITSET_LIB

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "error.h"

//#define USE_INLINE

/**
 * @brief Typ bitového pole(pro předávání parametru do funkce odkazem)
 * 
 */
typedef unsigned long bitset_t;

/**
 * @brief Typ indexu do bitového pole.
 * 
 */
typedef unsigned long bitset_index_t;

//! Makra
/**
 * @brief Makro které vrací kolik bitů má unsigned long ("32" nebo "64", záleží na překladači)
 * 
 */
#define ULONG_32_64_MAX (sizeof(bitset_t)*(__CHAR_BIT__))
//#define ULONG_32_64_MAX 64

/**
 * @brief Jeli výskyt zbytku po dělení vrátí "1" jinak "0"
 * 
 */
#define CF(vel) (vel%ULONG_32_64_MAX)? (1):(0)

/**
 * @brief Výpočet jak velké pole bude potřeba pro uložení všech bitů
 * 
 */
#define VELIKOST_POLE(vel) ((vel / (ULONG_32_64_MAX)) + (CF(vel)) + 1)

//TODO static_assert na 0 < velikost < ULONG_32_64_MAX, jmeno_pole není prázdné

/**
 * @brief Definuje proměnnou jmeno_pole
 * 
 * @param jmeno_pole    Název pole které se vytvoří a vynuluje (zachovejte všechna pravidla pro názvy proměných)
 * @param velikost      Velikost pole v bitech
 */
#define bitset_create(jmeno_pole,velikost)\
    static_assert((UINT_FAST64_MAX > (bitset_t)(velikost)),"bitset_create: Chyba alokace paměti"); \
    static_assert((0 < (velikost)),"bitset_create: Chyba při překladu"); \
    bitset_t (jmeno_pole)[VELIKOST_POLE((velikost))]; \
    (jmeno_pole)[0] = (velikost); \
    for (bitset_t i = 1; i < VELIKOST_POLE((velikost)); i++) { \
        (jmeno_pole)[i] = 0; \
    } \

/**
 * @brief Definuje dinamicky alokovanou proměnou jmeno_pole
 * 
 * @param jmeno_pole    Název pole které se vytvoří a vynuluje (zachovejte všechna pravidla pro názvy proměných)
 * @param velikost      Velikost pole v bitech
 */
#define bitset_alloc(jmeno_pole,velikost) \
    assert(UINT_FAST64_MAX > (bitset_t)(velikost)); \
    assert(0 < (velikost)); \
    bitset_t *(jmeno_pole) = (bitset_t*) calloc(VELIKOST_POLE((velikost)) ,sizeof(bitset_t)); \
    if ((jmeno_pole) == NULL) { \
        error_exit("bitset_alloc: Chyba alokace paměti",0); \
    } \
    (jmeno_pole)[0] = velikost \

#ifndef USE_INLINE

/**
 * @brief Uvolní dinamicky alokovanou proměnou jmeno_pole
 * 
 * @param jmeno_pole    Název pole k uvolnění
 */
#define bitset_free(jmeno_pole) free((jmeno_pole))

/**
 * @brief Vrací velikost jmeno_pole v bitech
 * 
 * @param jmeno_pole    Název pole kterého vrátí velikost
 */
#define bitset_size(jmeno_pole) ((jmeno_pole)[0])

/**
 * @brief Nastaví daný bit daného pole na danou hodnotu
 * 
 * @param jmeno_pole    Název pole kterému chceme nastavit bit
 * @param index         Bit na tom to indexu bude nastaven
 * @param vyraz         Hodnutu (0,1) na kterou se má index nastavit
 */
#define bitset_setbit(jmeno_pole,index,vyraz) do { \
    if ((bitset_size((jmeno_pole))) <= (bitset_t)(index)) \
        error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu\n",(unsigned long)(index), (unsigned long)bitset_size((jmeno_pole))); \
    if (vyraz) { \
        (jmeno_pole)[((index)/ULONG_32_64_MAX)+1] |= (bitset_t)1UL << ((index)%ULONG_32_64_MAX);\
    } else {\
        (jmeno_pole)[((index)/ULONG_32_64_MAX)+1] &= ~((bitset_t)1UL << ((index)%ULONG_32_64_MAX)); \
    } \
} while (0)

/**
 * @brief Vrací hodnotu daného bitu (1:0)
 * 
 * @param jmeno_pole    Název pole u kterého chceme zjistit hodnotu bitu
 * @param index         Index bitu kterého chceme znát hodnotu
 */
#define bitset_getbit(jmeno_pole,index) ((jmeno_pole)[((index)/ULONG_32_64_MAX)+1] & ((bitset_t)1UL << ((index)%ULONG_32_64_MAX))) ? (1) : (0)

//#endif  //USE_INLINE NOT DEFINE 

//! INLINE FUNKCE
#else

//#ifdef USE_INLINE
/**
 * @brief Uvolní dinamicky alokovanou proměnou jmeno_pole --inline
 * 
 * @param jmeno_pole    Název pole k uvolnění
 */
extern void bitset_free(bitset_t *jmeno_pole);

/**
 * @brief Vrací velikost jmeno_pole v bitech --inline
 * 
 * @param jmeno_pole    Název pole kterého vrátí velikost
 */
extern bitset_t bitset_size(bitset_t *jmeno_pole);

/**
 * @brief Nastaví daný bit daného pole na danou hodnotu --inline
 * 
 * @param jmeno_pole    Název pole kterému chceme nastavit bit
 * @param index         Bit na tom to indexu bude nastaven
 * @param vyraz         Hodnutu (0,1) na kterou se má index nastavit
 */
extern void bitset_setbit(bitset_t *jmeno_pole,bitset_index_t index,bool vyraz);

/**
 * @brief Vrací hodnotu daného bitu (1:0) (true:false) --inline
 * 
 * @param jmeno_pole    Název pole u kterého chceme zjistit hodnotu bitu
 * @param index         Index bitu kterého chceme znát hodnotu
 */
extern bool bitset_getbit(bitset_t *jmeno_pole,bitset_index_t index);

#endif  //USE_INLINE

#endif  // IJC_BITSET_LIB