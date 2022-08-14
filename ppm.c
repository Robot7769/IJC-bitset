/**
 * @file ppm.c
 * @author Jan Škrabal (xskrab12)-FIT
 * @brief IJC-DU1 B funkce nad odrázkem ve formátu .ppm
 * @date 2022-02-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ppm.h"

struct ppm *ppm_read(const char * filename){
    int delka = strlen(filename);
    char koncovka[5];
    if (delka > 4) {
        for (int i = delka-4,j = 0; i < delka+1; i++,j++){
            koncovka[j] = filename[i];
        }
    }
    if (strcmp(koncovka,".ppm")){
        //printf("koncovka souboru je .ppm");
        warning_msg("ppm_read: Koncovka obrázku není .ppm\n",0);
        return NULL;
    }
    
    FILE *obr;
    obr = fopen(filename,"rb");             // otevření souburu "filename"
    if (obr == NULL) {
        warning_msg("ppm_read: Chyba souboru '%s'\n",filename);
        return(NULL);
    }

    //!kontrola formátu ppm obrázku binární "P6"
    int magic[3] = {0,};
    for (size_t i = 0; i < 3; i++) {
        magic[i] = fgetc(obr);
    }
    
    if (!strcmp(MAGIC_NUM,(const char *)magic)) {
        fclose(obr);
        warning_msg("ppm_read: Nepotporovyný formát obrázku '%s'\n",magic);
        return(NULL);
    }
    int x,y,barva;
    if (fscanf(obr, "%d %d\n%d\n",&x,&y,&barva) != 3) {
        fclose(obr);
        warning_msg("ppm_read: Nesprávná hlavička souboru\n",0);
        return(NULL);
    }
    if (x < 1 || y < 1) {
        fclose(obr);
        warning_msg("ppm_read: Nesprávná velikost obrázku. X = %d, Y = %d\n",x,y);
        return(NULL);
    }
    if (barva != ROZSAH_BAREV) {
        fclose(obr);
        warning_msg("ppm_read: Nesprávný rozsah barev. %d != 255\n",barva);
        return(NULL);
    }
    if (LIMIT < (x*y*3)) {
        fclose(obr);
        warning_msg("ppm_read: Obrázek je příliž velký.\n",0);
        return(NULL);
    }
    struct ppm *obraz = malloc(3*x*y+sizeof(struct ppm));       // alokace paměti pro obrazová data a halvičku
    if (obraz == NULL) {
        warning_msg("ppm_read: Chyba alokace struktury\n",0);
        fclose(obr);
        return(NULL);
    }
    //! předání velikosti do struktury
    obraz->xsize = x;
    obraz->ysize = y;

    //! zbytek souboru (obrazová data) načte do obraz->data
    int znak;
    for (size_t i = 0;(znak = fgetc(obr) )!= EOF;i++) {
        obraz->data[i] = znak;
    }
    
    fclose(obr);
    return obraz;
}

int ppm_write(const char * filename, struct ppm *p){
    FILE *obr;
    obr = fopen(filename,"wb");             // otevření souburu "filename"
    if (obr == NULL) {
        warning_msg("ppm_write: Chyba souboru '%s'\n",filename);
        return 1;
    }
    fprintf(obr,"%s\n%d %d\n%d\n",MAGIC_NUM,(p->xsize),(p->ysize),ROZSAH_BAREV);
    //printf("file write\n");
    //printf("x: %d",100);
    for (size_t pos = 0;pos < (long)3*(p->xsize)*(p->ysize);pos++){
        fputc(p->data[pos],obr);
        //fprintf(obr,"%c",p->data[pos]);
    }
    fclose(obr);
    return 0;
}

void ppm_free(struct ppm *p){
    if (p != NULL) free(p);     // pokud není ppm strikuta prázdná, tak se uvolní
}