#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include "constantes.h"
#include "utilitaires.h"
#include "donnees_client.h"
#include "constituer_msg_tous.h"
#include "lire_msg_tous.h"
#include "traiter_commandes_client.h"

void traiter_commande_bcst(char* mess) {
  char mess_envoi[TAILLE_MAX];
  int ecrit;
    if (MON_DEBUG) printf("j'execute la commande bcst\n");
  constituer_msg_bcst_client(mess_envoi,ident,mess);
    if (MON_DEBUG) printf("je vais envoyer le message suivant: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(dtube_Sw,mess_envoi,longueur(mess_envoi));
  if(ecrit==-1) printf("erreur dans l'ecriture de mess_envoi de longueur %d\n",longueur(mess_envoi));
    if (MON_DEBUG) printf("j'ai envoyé: \n");
    if (MON_DEBUG) afficher(mess_envoi);
}

void traiter_commande_list() {
  char mess_envoi[TAILLE_MAX];
  int ecrit;
    if (MON_DEBUG) printf("j'execute la commande list\n");
  constituer_msg_list_client(mess_envoi,ident);
    if (MON_DEBUG) printf("je vais envoyer le message suivant: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(dtube_Sw,mess_envoi,longueur(mess_envoi));
  if(ecrit==-1) printf("erreur dans l'ecriture de mess_envoi de longueur %d\n",longueur(mess_envoi));
    if (MON_DEBUG) printf("j'ai envoyé: \n");
    if (MON_DEBUG) afficher(mess_envoi);
}

void traiter_commande_byee() {
  char mess_envoi[TAILLE_MAX];
  int ecrit;
    if (MON_DEBUG) printf("j'execute la commande byee\n");
  constituer_msg_byee(mess_envoi,ident);
    if (MON_DEBUG) printf("je vais envoyer le message suivant: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(dtube_Sw,mess_envoi,longueur(mess_envoi));
  if(ecrit==-1) printf("erreur dans l'ecriture de mess_envoi de longueur %d\n",longueur(mess_envoi));
    if (MON_DEBUG) printf("j'ai envoyé: \n");
    if (MON_DEBUG) afficher(mess_envoi);
}

void traiter_commande_shut() {
  char mess_envoi[TAILLE_MAX];
  int ecrit;
    if (MON_DEBUG) printf("j'execute la commande shut\n");
  constituer_msg_shut_client(mess_envoi,ident);
    if (MON_DEBUG) printf("je vais envoyer le message suivant: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(dtube_Sw,mess_envoi,longueur(mess_envoi));
  if(ecrit==-1) printf("erreur dans l'ecriture de mess_envoi de longueur %d\n",longueur(mess_envoi));
    if (MON_DEBUG) printf("j'ai envoyé: \n");
    if (MON_DEBUG) afficher(mess_envoi);
}

void traiter_commande_who() {
  printf("%s\n",mon_pseudo);
}



void traiter_commande_prvt(char* pseudo,char* mess) {
  char mess_envoi[TAILLE_MAX];
  int ecrit;
    if (MON_DEBUG) printf("j'execute la commande prvt\n");
  constituer_msg_prvt_client(mess_envoi,ident,pseudo,mess);
    if (MON_DEBUG) printf("je vais envoyer le message suivant: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(dtube_Sw,mess_envoi,longueur(mess_envoi));
  if(ecrit==-1) printf("erreur dans l'ecriture de mess_envoi de longueur %d\n",longueur(mess_envoi));
    if (MON_DEBUG) printf("j'ai envoyé: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  //Attention ne pas enlever ce printf
  printf("[%s --> %s] %s\n",mon_pseudo,pseudo,mess);
}

void traiter_commande_sendfile(char* nom_fichier,char * pseudo) {
  char mess_envoi[TAILLE_MAX];
  int serie=0, long_fichier, ecrit;
  struct stat s;
     if (MON_DEBUG) {printf("j'execute la commande senfile\n");
                     printf("voici le nom de fichier %s\n",nom_fichier);
                     printf("voici le destinataire %s\n",pseudo);}

  if (strlen(nom_fichier)==0) {printf("--pas de nom de fichier specifie\n"); return;}
  if (strlen(pseudo)==0) {printf("--pas de destinataire specifie\n"); return;}
  if (access(nom_fichier,F_OK)!=0) {printf("--pas acces au fichier %s\n",nom_fichier); return;}
  if (access(nom_fichier,R_OK)!=0) {printf("--pas le droit de lire le fichier %s\n",nom_fichier); return;}
  if (stat(nom_fichier,&s)==0) {long_fichier= (int)s.st_size;}
  else {printf("erreur a la lecture du fichier\n");return;}
  constituer_msg_sendfile_client(mess_envoi,serie,ident,pseudo,long_fichier,nom_fichier);
     if (MON_DEBUG) printf("je vais envoyer le message suivant: \n");
     if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(dtube_Sw,mess_envoi,longueur(mess_envoi));
  if(ecrit==-1) printf("erreur dans l'ecriture de mess_envoi de longueur %d\n",longueur(mess_envoi));
     if (MON_DEBUG) printf("j'ai envoyé: \n");
     if (MON_DEBUG) afficher(mess_envoi);
  transfert_demande_par_moi=1;
  strcpy(fichier_en_emission,nom_fichier);
  long_fichier_en_emission=long_fichier;
}


void traiter_commande_client(char* mess) {
     if (MON_DEBUG) printf("Voici la ligne de commande: %s \n",mess);
  char argv0[TAILLE_MAX],argv1[TAILLE_MAX],argv2[TAILLE_MAX];
  int ind,i=0;

 if (strlen(mess)==0) {printf("-en ecoute\n");return;}
  while (mess[i]!='\0' && mess[i]==' ') i++;
  while (mess[i]!='\0' && mess[i]!=' ') {argv0[i]=mess[i];i++;}
  argv0[i]='\0';

if (strcmp(argv0,"list")==0) {traiter_commande_list();return;}
else if (strcmp(argv0,"quit")==0) {traiter_commande_byee();return;}
else if (strcmp(argv0,"shut")==0) {traiter_commande_shut();return;}
else if (strcmp(argv0,"who")==0) {traiter_commande_who();return;}
else if (strcmp(argv0,"help")==0) {
  printf("commandes: \n who\n list\n private destinataire message\n sendfile fichier destinataire\n quit\n shut\n -par defaut broadcast\n");
  return;
}
else if (strcmp(argv0,"private")==0) {
  while (mess[i]!='\0' && mess[i]==' ') i++;
  ind=0;
  while (mess[i]!='\0' && mess[i]!=' ') {argv1[ind]=mess[i];i++;ind++;}
  argv1[ind]='\0';
  while (mess[i]!='\0' && mess[i]==' ') i++;
  ind=0;
  while (mess[i]!='\0' && mess[i]!='\0') {argv2[ind]=mess[i];i++;ind++;}
  argv2[ind]='\0';
  traiter_commande_prvt(argv1,argv2);
  return;}
else if (strcmp(argv0,"sendfile")==0) {
  while (mess[i]!='\0' && mess[i]==' ') i++;
  ind=0;
  while (mess[i]!='\0' && mess[i]!=' ') {argv1[ind]=mess[i];i++;ind++;}
  argv1[ind]='\0';
  while (mess[i]!='\0' && mess[i]==' ') i++;
  ind=0;
  while (mess[i]!='\0' && mess[i]!='\0') {argv2[ind]=mess[i];i++;ind++;}
  argv2[ind]='\0';
  traiter_commande_sendfile(argv1,argv2);
  return;}
else traiter_commande_bcst(mess);
return;

}
