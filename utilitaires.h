
#ifndef _UTILITAIRES_H
#define _UTILITAIRES_H
#define MON_DEBUG 0
#include "constantes.h"
char le_tube_serveur[TAILLE_MAX];
int goodbye_serveur;
int dtube_Sr;

typedef enum type_commande type_commande;
enum type_commande
{
  HELO,OKOK,BADD,BYEE,BCST,LIST,PRVT,SHUT,FILEP,NONE
};

type_commande type_message(char* mess);
void vider_buffer();
int lire(char* chaine, int longueur);
#endif
