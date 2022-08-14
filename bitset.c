/**
 * @file bitset.c
 * @author Jan Škrabal (xskrab12)-FIT
 * @brief IJC-DU1 A makra bitset
 * @date 2022-02-22
 *
 * @copyright Copyright (c) 2022
 *
 */


#include "bitset.h"

#ifdef USE_INLINE

extern inline void bitset_free(bitset_t *jmeno_pole){
    free(jmeno_pole);
}


extern inline bitset_t bitset_size(bitset_t *jmeno_pole){
    return jmeno_pole[0];
}

extern inline void bitset_setbit(bitset_t *jmeno_pole,bitset_index_t index,bool vyraz) {
    if ((bitset_size((jmeno_pole))) <= (index))
        error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu\n",(unsigned long)(index), (unsigned long)bitset_size((jmeno_pole)));
    if (vyraz) {
        (jmeno_pole)[((index)/ULONG_32_64_MAX)+1] |= (bitset_t)1UL << ((index)%ULONG_32_64_MAX);
    } else {
        (jmeno_pole)[((index)/ULONG_32_64_MAX)+1] &= ~((bitset_t)1UL << ((index)%ULONG_32_64_MAX));
    }
}

extern inline bool bitset_getbit(bitset_t *jmeno_pole,bitset_index_t index) {
    if ((bitset_size((jmeno_pole))) < (index))
        error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu\n",(unsigned long)(index), (unsigned long)bitset_size((jmeno_pole)));
    if ((jmeno_pole)[((index)/ULONG_32_64_MAX)+1] & ((bitset_t)1UL << ((index)%ULONG_32_64_MAX)))
        return 1;
    return 0;    

}


#endif //USE_INLINE
