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
#include "traiter_messages_serveur.h"
#include <sys/select.h>

#include <signal.h>
void f(int sig) {
    printf("--signal %d recu\n",sig);
  }
///////////////////////////////////////
int main(int argc, char* argv[]) {
struct sigaction action;
action.sa_handler=&f;
sigemptyset(&(action.sa_mask));
action.sa_flags=0;
sigaction(SIGPIPE,&action,NULL);

char mess_envoi[TAILLE_MAX], mess_recu[TAILLE_MESS_RECU], mess_a_traiter[TAILLE_MAX];
int res_S,res_C,dtube_Cw, lu, indice_courant;

strcpy(le_tube_serveur, LE_TUBE_SERVEUR);
goodbye_serveur=0;
   if (MON_DEBUG) printf("le nom du tube serveur est : %s \n",le_tube_serveur);
res_S=mkfifo(le_tube_serveur, S_IRWXU | S_IRGRP | S_IWGRP);
   if (res_S == -1) {
            printf("--le tube %s existe sans doute deja\n-en service\n",le_tube_serveur);
         } else
         {
            printf("--reussi a creer le tube serveur %s \n-en service\n",le_tube_serveur);
         }
   dtube_Sr=open(le_tube_serveur,O_RDONLY|O_NONBLOCK);
   if (dtube_Sr==-1) printf("erreur dans open 1\n");

initialiser_clients();
initialiser_transferts();
fd_set ensemble;
int nset;
   while (1) {
    FD_ZERO(&ensemble);
    FD_SET(dtube_Sr,&ensemble);
    nset=select(dtube_Sr+1,&ensemble,NULL,NULL,NULL);
    if (nset>0) {
      if(FD_ISSET(dtube_Sr,&ensemble)) {
        lu=read(dtube_Sr,mess_recu,TAILLE_MESS_RECU);
        if (lu>0) {
          //printf("caracteres lus: %d \n",lu);
          if (MON_DEBUG) printf("caracteres lus: %d \n",lu);
          if (MON_DEBUG) afficher_tout(mess_recu,lu);
        int indice_courant=0;
        while (lecture_messages(mess_recu,lu,mess_a_traiter,&indice_courant)) {
           if (MON_DEBUG) printf("voici ce que j'extraie et que je vais traiter: \n");
           if (MON_DEBUG) afficher(mess_a_traiter);
         traiter_message_serveur(mess_a_traiter);

        }
           if (MON_DEBUG) printf("termine \n");
         //break;
     }
   }
   }
   if (goodbye_serveur)
   {
     close(dtube_Sr);
     printf("Terminaison du serveur demandee\n");
     break;
   }
} //while
return 0;
}
