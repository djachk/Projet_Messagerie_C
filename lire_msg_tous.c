#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include "constantes.h"
#include "utilitaires.h"
#include "constituer_msg_tous.h"
#include "lire_msg_tous.h"

int lire_msg_helo(char* res, char* pseudo,char* tube) {
  char pour_long[4];
  int i,ind,long_pseudo,long_tube;
  if (strlen(res)<12) {printf("--message recu incorrect\n");return -1;}
  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  ind=12;

  long_pseudo=longueur(pour_long);
  if (strlen(res)<long_pseudo+ind) {printf("--message recu incorrect\n");return -1;}

  for (i=0;i<long_pseudo;i++) {
    pseudo[i]=res[ind+i];
  }
  pseudo[long_pseudo]='\0';
  ind += long_pseudo;

  if (strlen(res)<(4+ind)) {printf("--message recu incorrect\n");return -1;}  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  ind += 4;

  long_tube=longueur(pour_long);
  if (strlen(res)<(long_tube+ind)) {printf("--message recu incorrect\n");return -1;}
  for (i=0;i<long_tube;i++) {
    tube[i]=res[ind+i];
  }
  tube[long_tube]='\0';
return 0;
}

int lire_msg_okok(char* res, int* id) {
  char pour_long[4];
  if (strlen(res)<12) {printf("--message recu incorrect\n");return -1;}
  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *id = longueur(pour_long);
  return 0;
}

int lire_msg_bcst_serveur(char* res, int* id, char* mess) {
  char pour_long[4];
  int i,ltotale, lmess,ind;
     if (MON_DEBUG) afficher(res);
  if (strlen(res)<12) {printf("--message recu incorrect\n");return -1;}
  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *id = longueur(pour_long);
  ltotale=longueur(res);
  ind=16;
  lmess=ltotale-ind;
  if (strlen(res)<(lmess+ind)) {printf("--message recu incorrect\n");return -1;}
  for (i=0;i<lmess;i++) {
    mess[i]=res[ind+i];
  }
  mess[lmess]='\0';
  return 0;
}

int lire_msg_bcst_client(char* res, char* pseudo, char* mess) {
  char pour_long[4];
  int i,ind,long_pseudo,long_mess;
  if (strlen(res)<12) {printf("--message recu incorrect\n");return -1;}
  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  ind=12;

  long_pseudo=longueur(pour_long);
  if (strlen(res)<(long_pseudo+ind)) {printf("--message recu incorrect\n");return -1;}
  for (i=0;i<long_pseudo;i++) {
    pseudo[i]=res[ind+i];
  }
  pseudo[long_pseudo]='\0';
  ind += long_pseudo;

  if (strlen(res)<(4+ind)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  ind += 4;

  long_mess=longueur(pour_long);

  if (strlen(res)<(long_mess+ind)) {printf("--message recu incorrect\n");return -1;}

  for (i=0;i<long_mess;i++) {
    mess[i]=res[ind+i];
  }
  mess[long_mess]='\0';
  return 0;
}

int lire_msg_list_serveur(char* res, int* id) {
  char pour_long[4];
  if (strlen(res)<(12)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *id = longueur(pour_long);
  return 0;
}

int lire_msg_list_client(char* res, int* n, char* pseudo) {
  char pour_long[4];
  int i,ltotale, lpseudo,ind;
  if (strlen(res)<(12)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *n = longueur(pour_long);
  ltotale=longueur(res);
  ind=16;
  lpseudo=ltotale-ind;
  if (strlen(res)<(lpseudo+ind)) {printf("--message recu incorrect\n");return -1;}

  for (i=0;i<lpseudo;i++) {
    pseudo[i]=res[ind+i];
  }
  pseudo[lpseudo]='\0';
  return 0;
}

int lire_msg_byee_serveur(char* res, int* id) {
  char pour_long[4];
  if (strlen(res)<(12)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *id = longueur(pour_long);
  return 0;
}

int lire_msg_prvt_serveur(char* res, int* id, char* pseudo, char* mess){
  char pour_long[4];
  int i,ltotale, lpseudo,lmess,ind;
  if (strlen(res)<(12)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *id = longueur(pour_long);

  ind=12;
  if (strlen(res)<(16)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  ind += 4;

  lpseudo=longueur(pour_long);
  if (strlen(res)<(lpseudo+ind)) {printf("--message recu incorrect\n");return -1;}

  for (i=0;i<lpseudo;i++) {
    pseudo[i]=res[ind+i];
  }
  pseudo[lpseudo]='\0';
  ind += lpseudo;
  if (strlen(res)<(4+ind)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  ind += 4;

  lmess=longueur(pour_long);
  if (strlen(res)<(lmess+ind)) {printf("--message recu incorrect\n");return -1;}

  for (i=0;i<lmess;i++) {
    mess[i]=res[ind+i];
  }
  mess[lmess]='\0';
  return 0;
}

int lire_msg_prvt_client(char* res, char* pseudo, char* mess) {
  char pour_long[4];
  int i,ind,long_pseudo,long_mess;
  if (strlen(res)<(12)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  ind=12;

  long_pseudo=longueur(pour_long);
  if (strlen(res)<(long_pseudo+ind)) {printf("--message recu incorrect\n");return -1;}

  for (i=0;i<long_pseudo;i++) {
    pseudo[i]=res[ind+i];
  }
  pseudo[long_pseudo]='\0';
  ind += long_pseudo;
  if (strlen(res)<(4+ind)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  ind += 4;

  long_mess=longueur(pour_long);
  if (strlen(res)<(long_mess+ind)) {printf("--message recu incorrect\n");return -1;}

  for (i=0;i<long_mess;i++) {
    mess[i]=res[ind+i];
  }
  mess[long_mess]='\0';
  return 0;
}

int lire_msg_shut_serveur(char* res, int* id) {
  char pour_long[4];
  if (strlen(res)<(12)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *id = longueur(pour_long);
  return 0;
}

int lire_msg_shut_client(char* res, char* pseudo) {
  char pour_long[4];
  int i,ind,long_pseudo,long_mess;
  if (strlen(res)<(12)) {printf("--message recu incorrect\n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  ind=12;

  long_pseudo=longueur(pour_long);
  if (strlen(res)<(long_pseudo+ind)) {printf("--message recu incorrect\n");return -1;}

  for (i=0;i<long_pseudo;i++) {
    pseudo[i]=res[ind+i];
  }
  pseudo[long_pseudo]='\0';
  return 0;
}




int lire_msg_sendfile_serveur(char* res, int* serie,int* id_client, char* pseudo_dest,int* long_fichier,char* nom_de_fichier) {
  char pour_long[8];
  int i,ltotale, lpseudo,lnomf,ind;
  if (strlen(res)<(16)) {printf("--message recu incorrect \n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *serie = longueur(pour_long);
  ind=12;

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  *id_client=longueur(pour_long);
  ind += 4;

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  ind += 4;

  lpseudo=longueur(pour_long);
  if (strlen(res)<(lpseudo+ind)) {printf("--message recu incorrect \n");return -1;}

  for (i=0;i<lpseudo;i++) {
    pseudo_dest[i]=res[ind+i];
  }
  pseudo_dest[lpseudo]='\0';
  ind += lpseudo;
  if (strlen(res)<(8+ind)) {printf("--message recu incorrect \n");return -1;}

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  pour_long[4]=res[ind+4];
  pour_long[5]=res[ind+5];
  pour_long[6]=res[ind+6];
  pour_long[7]=res[ind+7];
  *long_fichier=longueur_8(pour_long);
  ind += 8;

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  ind += 4;

  lnomf=longueur(pour_long);
  if (strlen(res)<(lnomf+ind)) {printf("--message recu incorrect \n");return -1;}

  for (i=0;i<lnomf;i++) {
    nom_de_fichier[i]=res[ind+i];
  }
  nom_de_fichier[lnomf]='\0';
  return 0;
}

int lire_msg_sendfile_client(char* res, int* serie,int* id_transfert, int* long_fichier,char* nom_de_fichier) {
  char pour_long[8];
  int i,ltotale, lpseudo,lnomf,ind;
  if (strlen(res)<(20)) {printf("--message recu incorrect \n");return -1;}

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *serie = longueur(pour_long);
  ind=12;

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  *id_transfert=longueur(pour_long);
  ind += 4;

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  pour_long[4]=res[ind+4];
  pour_long[5]=res[ind+5];
  pour_long[6]=res[ind+6];
  pour_long[7]=res[ind+7];
  *long_fichier=longueur_8(pour_long);
  ind += 8;


  if (strlen(res)<(4+ind)) {printf("--message recu incorrect \n");return -1;}

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  ind += 4;

  lnomf=longueur(pour_long);
  if (strlen(res)<(lnomf+ind)) {printf("--message recu incorrect \n");return -1;}

  for (i=0;i<lnomf;i++) {
    nom_de_fichier[i]=res[ind+i];
  }
  nom_de_fichier[lnomf]='\0';
  return 0;
}


int lire_msg_paquet(char* res,int* serie,int* ind_transfert,char* donnees, int* taille) {
  char pour_long[4];
  int i,ltotale, lpseudo,lnomf,ind;
  if (strlen(res)<(16)) {printf("--message recu incorrect \n");return -1;}

  if (MON_DEBUG) {printf("dans lire_msg_paquet voici le message a traiter:\n");
     afficher(res);}

  pour_long[0]=res[0];
  pour_long[1]=res[1];
  pour_long[2]=res[2];
  pour_long[3]=res[3];
  ltotale = longueur(pour_long);

  pour_long[0]=res[8];
  pour_long[1]=res[9];
  pour_long[2]=res[10];
  pour_long[3]=res[11];
  *serie = longueur(pour_long);
  ind=12;

  pour_long[0]=res[ind+0];
  pour_long[1]=res[ind+1];
  pour_long[2]=res[ind+2];
  pour_long[3]=res[ind+3];
  *ind_transfert=longueur(pour_long);
  ind += 4;

  for (i=16;i<ltotale;i++) {
    donnees[i-16]=res[i];
  }
  *taille=ltotale-16;
  return 0;
}

/*void lire_msg_okok(char* res, int id) ;
void lire_msg_badd(char* res);
void lire_msg_byee(char* res, int id);

void lire_msg_bcst_serveur(char* res, char* pseudo,char* mess);
*/
