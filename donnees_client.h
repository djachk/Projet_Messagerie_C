#ifndef _DONNEES_CLIENT_H
#define _DONNEES_CLIENT_H
#include <stdio.h>
#include "constantes.h"
int ident;
int dtube_Cr, dtube_Sw;
int connexion_demandee;
int goodbye;
int transfert_demande_par_moi,mon_idtransfert,transfert_demande_par_src,son_idtransfert,perdu_paquet;
int no_serie_en_reception, long_fichier_en_reception, long_fichier_en_emission;
char fichier_en_emission[TAILLE_MAX],fichier_en_reception[TAILLE_MAX];
FILE* frec;

char mon_tube[L_tmpnam];
//char mon_tube[TAILLE_MAX];
char mon_pseudo[TAILLE_MAX];
#endif
