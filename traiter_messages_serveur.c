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
#include "traiter_messages_serveur.h"

typedef struct client client;
struct client
{
  int occupe;
  char* pseudo;
  char * tube;
  int id;
  int descr;
};
typedef struct transfert transfert;
struct transfert
{
  int id_source;
  int id_dest;
  char* nomf;
  int en_cours;
  int serie;
  int longf;
};
static int ind_transfert=0;
static int nb_transferts=0;
static transfert transferts[NB_TRANSFERTS_MAX];

static int nb_clients=0;
static int ind_client=0;
static client clients[NB_CLIENTS_MAX];

void initialiser_clients() {
  int i;
  for (i=0;i<NB_CLIENTS_MAX;i++) {
    clients[i].occupe=0;
  }
}

void initialiser_transferts() {
  int i;
  for (i=0;i<NB_TRANSFERTS_MAX;i++) {
    transferts[i].en_cours=0;
    transferts[i].serie=0;
  }
}

int prochain_libre() {
  int ind=-1,i;
  for (i=0;i<NB_CLIENTS_MAX;i++) {
    if (clients[i].occupe==0) {ind=i;break;}
  }
  return ind;
}

int client_par_pseudo(char* un_pseudo) {
  int ind=-1,i;
  for (i=0;i<NB_CLIENTS_MAX;i++) {
    if ((clients[i].occupe) && (strcmp(clients[i].pseudo,un_pseudo)==0)) {ind=i;break;}
  }
  return ind;
}

void envoyer_badd(int id_client) {
    if (MON_DEBUG) printf("j'envoie un badd\n");
  char mess_envoi[TAILLE_MAX];
  int i,ecrit;
  constituer_msg_badd(mess_envoi);
    if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(clients[id_client].descr,mess_envoi,longueur(mess_envoi));
    if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
  if(ecrit==-1)
  printf("erreur dans l'ecriture de mess_envoi au client\n");
}

void envoyer_badd_pseudo(int des) {
    if (MON_DEBUG) printf("j'envoie un badd\n");
  char mess_envoi[TAILLE_MAX];
  int i,ecrit;
  constituer_msg_badd(mess_envoi);
    if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(des,mess_envoi,longueur(mess_envoi));
    if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
  if(ecrit==-1)
  printf("erreur dans l'ecriture de mess_envoi au client\n");
}

void traiter_serveur_helo(char* mess) {

  char mess_envoi[TAILLE_MAX],pseudo_lu[TAILLE_MAX], tube_lu[TAILLE_MAX];
  int ecrit,lu,des;
  lu=lire_msg_helo(mess, pseudo_lu, tube_lu);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
    if (MON_DEBUG) printf("je lis pseudo= %s \n",pseudo_lu);
    if (MON_DEBUG) printf("je lis tube= %s \n",tube_lu);

  ind_client=prochain_libre();
  if (ind_client==-1) {printf("--plus de place pour creer un nouvel abonne\n"); return;}
  if (client_par_pseudo(pseudo_lu)!=-1) {
    printf("--le pseudo \"%s\" existe deja\n",pseudo_lu);
    des=open(tube_lu,O_WRONLY|O_NONBLOCK);
    envoyer_badd_pseudo(des);
    close(des);
    return;}
  clients[ind_client].pseudo=malloc(TAILLE_MAX*sizeof(char));
  strcpy(clients[ind_client].pseudo,pseudo_lu);
  clients[ind_client].tube=malloc(TAILLE_MAX*sizeof(char));
  strcpy(clients[ind_client].tube,tube_lu);
  clients[ind_client].id=ind_client;

    if (MON_DEBUG) printf("pseudo du client: %s \n",clients[ind_client].pseudo);
    if (MON_DEBUG) printf("tube du client: %s \n",clients[ind_client].tube);

  clients[ind_client].descr=open(clients[ind_client].tube,O_WRONLY|O_NONBLOCK);
  if (clients[ind_client].descr ==-1) {printf("--erreur dans open du tube du client\n");return;}
  constituer_msg_okok(mess_envoi,clients[ind_client].id);
    if (MON_DEBUG) printf("je vais envoyer au client le message: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(clients[ind_client].descr,mess_envoi,longueur(mess_envoi));
  printf("Nouvel abonné: %s\n",clients[ind_client].pseudo);
    if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
  if(ecrit==-1)
  {printf("--erreur dans l'ecriture de mess_envoi au client\n");return;}
  clients[ind_client].occupe=1;
  nb_clients++;
}

void  traiter_serveur_bcst(char* mess_a_traiter) {
   if (MON_DEBUG) printf("je traite le message bcst\n");
 int i,id_client, ecrit,lu;
 char mess[TAILLE_MAX],mess_envoi[TAILLE_MAX];
 lu=lire_msg_bcst_serveur(mess_a_traiter, &id_client, mess);
 if (lu==-1) {printf("--message recu incoherent\n"); return;}
 if ((id_client<0) || (id_client>=NB_CLIENTS_MAX) || (clients[id_client].occupe!=1))
    {printf("--client inconnu \n"); return;}
   if (MON_DEBUG) printf("pseudo du client: %s \n",clients[id_client].pseudo);
   if (MON_DEBUG) printf("message du client: %s \n",mess);
 constituer_msg_bcst_serveur(mess_envoi,clients[id_client].pseudo,mess);
 for (i=0;i<NB_CLIENTS_MAX;i++) {
   if (clients[i].occupe==1) {
       if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
       if (MON_DEBUG) afficher(mess_envoi);
     ecrit=write(clients[i].descr,mess_envoi,longueur(mess_envoi));
       if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
     if(ecrit==-1)
     printf("--erreur dans l'ecriture de mess_envoi au client\n");

   }
 }
}

void  traiter_serveur_list(char* mess_a_traiter) {
    if (MON_DEBUG) printf("je traite list\n");
  char mess_envoi[TAILLE_MAX];
  int i,id_client,ecrit,lu;
  lu=lire_msg_list_serveur(mess_a_traiter,&id_client);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
  if ((id_client<0) || (id_client>=NB_CLIENTS_MAX) || (clients[id_client].occupe!=1))
     {printf("--client inconnu \n"); return;}
  for (i=0;i<NB_CLIENTS_MAX;i++) {
    if (clients[i].occupe==1) {
      constituer_msg_list_serveur(mess_envoi,nb_clients,clients[i].pseudo);
        if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
        if (MON_DEBUG) afficher(mess_envoi);
      ecrit=write(clients[id_client].descr,mess_envoi,longueur(mess_envoi));
        if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
      if(ecrit==-1)
      printf("--erreur dans l'ecriture de mess_envoi au client\n");
    }
}
}

void  traiter_serveur_byee(char* mess_a_traiter) {
    if (MON_DEBUG) printf("je traite byee\n");
  char mess_envoi[TAILLE_MAX];
  int i,id_client,ecrit,lu;
  lu=lire_msg_list_serveur(mess_a_traiter,&id_client);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
  if ((id_client<0) || (id_client>=NB_CLIENTS_MAX) || (clients[id_client].occupe!=1))
     {printf("--client inconnu \n"); return;}
  constituer_msg_byee(mess_envoi,id_client);
    if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(clients[id_client].descr,mess_envoi,longueur(mess_envoi));
  printf("Départ de l'abonné: %s\n",clients[id_client].pseudo);
    if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
  if(ecrit==-1)
  printf("--erreur dans l'ecriture de mess_envoi au client\n");

  free(clients[id_client].pseudo);
  close(clients[id_client].descr);
  free(clients[id_client].tube);
  clients[id_client].occupe=0;
  nb_clients--;
}

void  traiter_serveur_prvt(char* mess_a_traiter) {
    if (MON_DEBUG) printf("je traite prvt\n");
  char mess_envoi[TAILLE_MAX], mess[TAILLE_MAX],pseudo[TAILLE_MAX];
  int i,id_client,id_dest, ecrit,lu;
    if (MON_DEBUG) afficher(mess_a_traiter);
  lu=lire_msg_prvt_serveur(mess_a_traiter,&id_client, pseudo, mess);
  if (lu==-1) {printf("--message recu incoherent\n"); return;}
  if ((id_client<0) || (id_client>=NB_CLIENTS_MAX) || (clients[id_client].occupe!=1))
     {printf("--client inconnu \n"); return;}
    if (MON_DEBUG) printf("voici le pseudo que je cherche %s \n", pseudo);
  id_dest=client_par_pseudo(pseudo);
    if (MON_DEBUG) printf("voici l'id que j'ai trouve, id_dest= %d \n", id_dest);
  if (id_dest==-1) {printf("--client inconnu \n"); envoyer_badd(id_client); return;}

  constituer_msg_prvt_serveur(mess_envoi,clients[id_client].pseudo,mess);
    if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
    if (MON_DEBUG) afficher(mess_envoi);
  ecrit=write(clients[id_dest].descr,mess_envoi,longueur(mess_envoi));
    if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
  if(ecrit==-1)
  printf("--erreur dans l'ecriture de mess_envoi au client\n");

}

void  traiter_serveur_shut(char* mess_a_traiter) {
  if (MON_DEBUG) printf("je traite le message shut\n");
int i,id_client, ecrit,lu;
char mess[TAILLE_MAX],mess_envoi[TAILLE_MAX];
lu=lire_msg_shut_serveur(mess_a_traiter, &id_client);
if (lu==-1) {printf("--message recu incoherent\n"); return;}
if ((id_client<0) || (id_client>=NB_CLIENTS_MAX) || (clients[id_client].occupe!=1))
   {printf("--client inconnu \n"); return;}
  if (MON_DEBUG) printf("pseudo du client: %s \n",clients[id_client].pseudo);
constituer_msg_shut_serveur(mess_envoi,clients[id_client].pseudo);

for (i=0;i<NB_CLIENTS_MAX;i++) {
  if (clients[i].occupe==1) {
      if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
      if (MON_DEBUG) afficher(mess_envoi);
    ecrit=write(clients[i].descr,mess_envoi,longueur(mess_envoi));
      if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
    if(ecrit==-1)
    printf("--erreur dans l'ecriture de mess_envoi au client\n");
   }
  }
  for (i=0;i<NB_CLIENTS_MAX;i++) {
    if (clients[i].occupe==1) {
        if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
        if (MON_DEBUG) afficher(mess_envoi);
        close(clients[i].descr);
        free(clients[i].pseudo);
        free(clients[i].tube);
        clients[i].occupe=0;
        goodbye_serveur=1;
     }
    }

}

void traiter_serveur_sendfile(char* mess_a_traiter) {

int i,ecrit,lu,serie,ind_client,long_fichier, ind_dest;
char mess[TAILLE_MAX],mess_envoi[TAILLE_MAX],pseudo_dest[TAILLE_MAX],nom_de_fichier[TAILLE_MAX];
    if (MON_DEBUG) {printf("je traite le message sendfile\n"); afficher(mess_a_traiter);}

if (lire_serie(mess_a_traiter)==0)  {
lu=lire_msg_sendfile_serveur(mess_a_traiter, &serie,&ind_client,pseudo_dest,&long_fichier,nom_de_fichier);
if (lu==-1) {printf("--message recu incoherent\n"); return;}
if (client_par_pseudo(pseudo_dest)==-1) {printf("--client inconnu\n"); envoyer_badd(ind_client);return;}
transferts[ind_transfert].en_cours=1;
transferts[ind_transfert].id_source=ind_client;
ind_dest=client_par_pseudo(pseudo_dest);
transferts[ind_transfert].id_dest=ind_dest;
transferts[ind_transfert].nomf=malloc(TAILLE_MAX*sizeof(char));
strcpy( transferts[ind_transfert].nomf ,pseudo_dest);
transferts[ind_transfert].serie=0;
transferts[ind_transfert].longf=long_fichier;
printf("--transfert de fichier demande\n");
  if (MON_DEBUG)
  {printf("nom de fichier recu %s \n",nom_de_fichier);
   printf("longueur de fichier recu %d \n",long_fichier); }

constituer_msg_senddest_serveur(mess_envoi,serie,ind_transfert,long_fichier,nom_de_fichier);
   if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
   if (MON_DEBUG) afficher(mess_envoi);
ecrit=write(clients[ind_dest].descr,mess_envoi,longueur(mess_envoi));
   if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
if(ecrit==-1)
printf("--erreur dans l'ecriture de mess_envoi au client\n");

constituer_msg_sendsrcok_serveur(mess_envoi,ind_transfert);
   if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
   if (MON_DEBUG) afficher(mess_envoi);
ecrit=write(clients[ind_client].descr,mess_envoi,longueur(mess_envoi));
   if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
if(ecrit==-1)
printf("--erreur dans l'ecriture de mess_envoi au client\n");
ind_transfert++;
if (ind_transfert==NB_TRANSFERTS_MAX) ind_transfert=0;
}
else //serie e
{
char donneesb[256],mess_envoib[TAILLE_MAX];
int serieb,ind_transfertb,tailleb;
if (MON_DEBUG)  {printf("je traite des paquets: voici le message que j'ai recu:\n");
  afficher(mess_a_traiter);}
lu=lire_msg_paquet(mess_a_traiter,&serieb,&ind_transfertb,donneesb,&tailleb);
if (lu==-1) {printf("--message recu incoherent\n"); return;}
constituer_msg_paquet(mess_envoib,serieb,ind_transfertb,donneesb,tailleb);
  if (MON_DEBUG) printf("je vais envoyer a un client le message: \n");
  if (MON_DEBUG) afficher(mess_envoib);
int iden_dest=transferts[ind_transfertb].id_dest;
if (MON_DEBUG) printf("reception et reenvoi du paquet no %d de taille %d\n",serieb,tailleb);
ecrit=write(clients[iden_dest].descr,mess_envoib,longueur(mess_a_traiter));
  if (MON_DEBUG) printf("j'ai envoyé au client le message susdit \n");
if(ecrit==-1)
printf("--erreur dans l'ecriture du message au client\n");
transferts[ind_transfertb].serie=serieb;
  if ((256*(serieb-1) + tailleb) == transferts[ind_transfertb].longf)
  {free(transferts[ind_transfertb].nomf);}
//mettre a jour serie dans le struct transferts
//regarder si termine et liberer

}

}


void traiter_message_serveur(char* mess_a_traiter) {
  int op;
  switch(type_message(mess_a_traiter))
  {
    case HELO:
    traiter_serveur_helo(mess_a_traiter);
    break;
    case BCST:
    traiter_serveur_bcst(mess_a_traiter);
    break;
    case LIST:
    traiter_serveur_list(mess_a_traiter);
    break;
    case BYEE:
    traiter_serveur_byee(mess_a_traiter);
    break;
    case PRVT:
    traiter_serveur_prvt(mess_a_traiter);
    break;
    case SHUT:
    traiter_serveur_shut(mess_a_traiter);
    break;
    case FILEP:
    traiter_serveur_sendfile(mess_a_traiter);
    break;
    default:
    printf("commande protocole inconnue\n");
    break;
  }
}
