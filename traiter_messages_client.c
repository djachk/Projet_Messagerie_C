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
#include "traiter_messages_client.h"


void traiter_client_okok(char* mess) {
  int lu, taille,serie=0,flagsblock, flagsnonblock,ecrit;
  char donnees[256], mess_envoi[TAILLE_MAX];

  if (transfert_demande_par_moi==0) {
  lu=lire_msg_okok(mess, &ident);
  if (lu==-1) {printf("message recu incoherent\n"); return;}
  connexion_demandee=0;
  printf("Connected. (%d) \n", ident);
  printf("-en ecoute\n");
  }
  else
  {
    if (MON_DEBUG) printf("je suis pret a transfere le fichier %s \n", fichier_en_emission);
    lu=lire_msg_okok(mess, &mon_idtransfert);
    if (lu==-1) {printf("--message recu incoherent\n"); return;}
    FILE* f=fopen(fichier_en_emission,"r");
    if (f==NULL) {printf("--impossible d'ouvrir le fichier %s \n",fichier_en_emission); return;}

    flagsnonblock = fcntl(dtube_Sw, F_GETFL, 0);
    flagsblock = flagsnonblock & (~O_NONBLOCK);
    fcntl(dtube_Sw, F_SETFL,flagsblock);
    serie=0;
    while(1) {
      if (MON_DEBUG) printf("j'essaie de lire\n");
      taille=fread(donnees,1,256,f);

      serie++;
    constituer_msg_paquet(mess_envoi,serie,mon_idtransfert,donnees, taille);
    if (MON_DEBUG) printf("envoi du paquet no : %d  de taille %d\n",serie, taille);
         if (MON_DEBUG) printf("je vais envoyer le message suivant: \n");
         if (MON_DEBUG) afficher(mess_envoi);
      ecrit=write(dtube_Sw,mess_envoi,longueur(mess_envoi));
      if(ecrit==-1) printf("erreur dans l'ecriture de mess_envoi de longueur %d\n",longueur(mess_envoi));
         if (MON_DEBUG) printf("j'ai envoyé: \n");
         if (MON_DEBUG) afficher(mess_envoi);
      if (taille<256) {
         printf("--envoi reussi du fichier %s \n", fichier_en_emission);
         break;}
    }
    fcntl(dtube_Sw,F_SETFL,flagsnonblock);
    fclose(f);
    transfert_demande_par_moi=0;
  }
}



void traiter_client_bcst(char* mess_a_traiter) {
  int lu;
    if (MON_DEBUG) printf("je traite bcst\n");
  char pseudo[TAILLE_MAX], mess[TAILLE_MAX];
  lu=lire_msg_bcst_client(mess_a_traiter, pseudo, mess);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
  //!!!!!ne pas enlever ce printf!!!
  printf("[%s] %s \n",pseudo,mess);
}

void traiter_client_list(char* mess_a_traiter) {
  int lu;
    if (MON_DEBUG) printf("je traite list\n");
  char pseudo[TAILLE_MAX];
  static int vu=0;
  int n;
  lu=lire_msg_list_client(mess_a_traiter, &n, pseudo);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
  //Attention ne pas enlever ce printf !!!!
  printf("[abonné en ligne: %s]\n",pseudo);
  vu++;
  if (vu==n) vu=0;
}

void traiter_client_byee(char* mess_a_traiter) {
    if (MON_DEBUG) printf("je traite byee\n");
  goodbye=1;
}

void traiter_client_prvt(char* mess_a_traiter) {
  int lu;
    if (MON_DEBUG) printf("je traite prvt\n");
  char pseudo[TAILLE_MAX], mess[TAILLE_MAX];
  lu=lire_msg_bcst_client(mess_a_traiter, pseudo, mess);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
  //!!!!!ne pas enlever ce printf!!!
  printf("[%s --> %s] %s \n",pseudo,mon_pseudo,mess);
}

void traiter_client_badd(char* mess_a_traiter) {
    if (MON_DEBUG) printf("je traite badd\n");
  //attention ne pas en lever ce printf !!!!
  printf("***operation impossible***\n");
  if (connexion_demandee) goodbye=1;
}

void traiter_client_shut(char* mess_a_traiter) {
  int lu;
    if (MON_DEBUG) printf("je traite shut\n");
  char pseudo[TAILLE_MAX];
  lu=lire_msg_shut_client(mess_a_traiter, pseudo);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
  //!!!!!ne pas enlever ce printf!!!
  printf("[%s] demande d'arret general\n",pseudo);
  goodbye=1;
}

void traiter_client_filep(char* mess_a_traiter) {
int lu,serie, id_transfert,long_fichier,taille,ecrits;
char nom_de_fichier[TAILLE_MAX],donnees[256];
   if (MON_DEBUG) printf("je traite filep\n");
if (lire_serie(mess_a_traiter)==0) {
  if (MON_DEBUG) printf("je vais creer un transfert \n");
lu=lire_msg_sendfile_client(mess_a_traiter, &serie,&id_transfert, &long_fichier,nom_de_fichier);
if (lu==-1) {printf("--message recu incoherent\n"); return;}
transfert_demande_par_src=1;
no_serie_en_reception=0;
perdu_paquet=0;
son_idtransfert=id_transfert;
long_fichier_en_reception=long_fichier;
strcpy(fichier_en_reception,nom_de_fichier);
strcat(fichier_en_reception,".copie");
frec=fopen(fichier_en_reception,"w");
if (frec==NULL) {printf("--impossible d'ouvrir le fichier pour ecriture\n"); return;}
  if (MON_DEBUG) {printf("voici le fichier que je vais ecrire %s de taille %d\n",fichier_en_reception,long_fichier);}
}
else
{
  if (MON_DEBUG) printf("je recois des paquets de fichier\n");
  lu=lire_msg_paquet(mess_a_traiter,&serie,&id_transfert,donnees,&taille);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
  ecrits=fwrite(donnees,1,taille,frec);
  if (ecrits!=taille) {printf("erreur d'ecriture dans le fichier %s\n",fichier_en_reception);}
  if (MON_DEBUG) printf("receptiion du paquet no %d: de taille %d\n",serie,taille);
  if (serie!=no_serie_en_reception+1) perdu_paquet=1;
  no_serie_en_reception=serie;
  if ((256*(serie-1) + taille) == long_fichier_en_reception) {
    printf("--reception terminée et ecriture du fichier %s\n",fichier_en_reception);
    if (perdu_paquet) printf("--attention, des paquets perdus! \n");
    fclose(frec);
    transfert_demande_par_src=0;
  }
}
}


void traiter_message_client(char* mess_a_traiter) {
  switch(type_message(mess_a_traiter))
  {
    case OKOK:
    traiter_client_okok(mess_a_traiter);
    break;
    case BCST:
    traiter_client_bcst(mess_a_traiter);
    break;
    case LIST:
    traiter_client_list(mess_a_traiter);
    break;
    case BYEE:
    traiter_client_byee(mess_a_traiter);
    break;
    case PRVT:
    traiter_client_prvt(mess_a_traiter);
    break;
    case BADD:
    traiter_client_badd(mess_a_traiter);
    break;
    case SHUT:
    traiter_client_shut(mess_a_traiter);
    break;
    case FILEP:
    traiter_client_filep(mess_a_traiter);
    break;
    default:
    printf("--commande protocole inconnue\n");
    break;
  }
}
