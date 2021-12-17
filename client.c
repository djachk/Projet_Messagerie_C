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
#include "traiter_messages_client.h"
#include "traiter_commandes_client.h"
#include <sys/select.h>

int main(int argc, char* argv[]) {

  char mess_envoi[TAILLE_MAX], mess_recu[TAILLE_MESS_RECU],mess_a_traiter[TAILLE_MAX];
  char mess_commande[TAILLE_MAX];
  int res_S,res_C,ecrit,lu1,lu2,lu_ecran,flagsblock,flagsnonblock;
  int id, indice_courant;
  char c;

//lecture du pseudo du client
  printf("Pseudo> ");
  lu_ecran=lire(mon_pseudo,TAILLE_MAX);
//creation du nom de son tube
  tmpnam(mon_tube);
    if (MON_DEBUG) printf("votre pseudo est: %s  \n",mon_pseudo);
    if (MON_DEBUG) printf("votre tube est: %s  \n",mon_tube);
//creation du tube
  res_C=mkfifo(mon_tube, S_IRWXU | S_IRGRP | S_IWGRP);
  if (res_C == -1)
    {printf("--le tube %s existe sans doute deja\n",mon_tube);}
  else
    {printf("--reussi a creer le tube client %s \n",mon_tube);}
//ouverture du tube par le client
   dtube_Cr=open(mon_tube, O_RDONLY|O_NONBLOCK);
   if (dtube_Cr==-1) printf("erreur dans open du tube client\n");
   else   {if (MON_DEBUG) printf("j'ai reussi a ouvrir tube client et descr= %d \n",dtube_Cr);}
   strcpy(le_tube_serveur, LE_TUBE_SERVEUR);
     if (MON_DEBUG) printf("le tube serveur est:  %s \n",le_tube_serveur);
//ouverture du tube serveur
   dtube_Sw=open(le_tube_serveur,O_WRONLY|O_NONBLOCK);
   if (dtube_Sw==-1) printf("erreur dans open du serveur\n");
   connexion_demandee=1;
//cobstitution du message protocole helo
   constituer_msg_helo(mess_envoi,mon_pseudo,mon_tube);
     if (MON_DEBUG) printf("je vais envoyer le message suivant: \n");
     if (MON_DEBUG) afficher(mess_envoi);
//envoi au serveur du message helo
   ecrit=write(dtube_Sw,mess_envoi,longueur(mess_envoi));
   if(ecrit==-1) printf("erreur dans l'ecriture de mess_envoi de longueur %d\n",longueur(mess_envoi));
     if (MON_DEBUG) printf("j'ai envoyé: \n");
     if (MON_DEBUG) afficher(mess_envoi);

//constitution des flags bloquants et non bloquants
   flagsblock = fcntl(0, F_GETFL, 0);
   flagsnonblock = flagsblock|O_NONBLOCK;
   fcntl(0, F_SETFL,flagsnonblock);

//positionnement de donnees client
  goodbye=0;
  transfert_demande_par_moi=0;
  transfert_demande_par_src=0;

  fd_set ensemble;
  int nset;
//boucle principale
  while(1) {
    FD_ZERO(&ensemble);
    FD_SET(dtube_Cr,&ensemble);
    FD_SET(0,&ensemble);
    //utilisation de select pour ne pas consommer de cpu
    nset=select(dtube_Cr+1,&ensemble,NULL,NULL,NULL);
    if (nset>0) {
      if(FD_ISSET(dtube_Cr,&ensemble)) {
         //on lit tout ce qu'il ya dans le tube client
         lu1=read(dtube_Cr,mess_recu,TAILLE_MESS_RECU);
         if (lu1>0) {
             if (MON_DEBUG) printf("j'ai recu le message : \n");
             if (MON_DEBUG) afficher_tout(mess_recu,lu1);
           indice_courant=0;
           //on reconstitue tous les messages de protocole conetnus
           while (lecture_messages(mess_recu,lu1,mess_a_traiter,&indice_courant)) {
                if (MON_DEBUG) printf("voici ce que j'extraie et que je vais traiter: \n");
                if (MON_DEBUG) afficher(mess_a_traiter);
              //on traite le message recu
              traiter_message_client(mess_a_traiter);
           }
           if(goodbye) {
             close(dtube_Cr);
             close(dtube_Sw);
             printf("Au revoir\n");
             break;
          }
         }
      }
      if(FD_ISSET(0,&ensemble))  {
         lu2=read(0,&c,1);
         //c'est une commande client
         if (lu2>0) {
           vider_buffer();
           //on bloque pour que le client puisse entrer sa commande
           fcntl(0, F_SETFL,flagsblock);
           printf("input> ");
           //on lit la commande client
           lire(mess_commande, TAILLE_MAX);
             if (MON_DEBUG) printf("vous avez tapé: %s \n",mess_commande);
           //on remet a non bloquant
           fcntl(0, F_SETFL,flagsnonblock);
           //on traite la commande du client
           traiter_commande_client(mess_commande);
           }
         }
      }// nset
    } //while

return 0;
}
