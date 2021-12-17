#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constantes.h"
#include "utilitaires.h"
#include "constituer_msg_tous.h"

//pour afficher
int longueur(char* mess) {
  return ((mess[0]-'0')*1000 + (mess[1]-'0')*100 + (mess[2]-'0')*10 + (mess[3]-'0'));
  }

int lire_serie(char* mess) {
    if(strlen(mess)<12) {printf("message reçu de taille incorrecte\n");return -1;}
    return ((mess[8]-'0')*1000 + (mess[9]-'0')*100 + (mess[10]-'0')*10 + (mess[11]-'0'));
}

int longueur_8(char* mess) {
  return ((mess[0]-'0')*10000000 + (mess[1]-'0')*1000000 + (mess[2]-'0')*100000 + (mess[3]-'0')*10000
     + (mess[4]-'0')*1000 + (mess[5]-'0')*100 + (mess[6]-'0')*10 + (mess[7]-'0'));
  }

int lecture_messages(char* mess_recu, int lu,char *mess_a_traiter,int* indice_courant) {
  int lon,i;
  if (lu>(*indice_courant)+3) {
    mess_a_traiter[0]=mess_recu[*indice_courant];
    mess_a_traiter[1]=mess_recu[(*indice_courant)+1];
    mess_a_traiter[2]=mess_recu[(*indice_courant)+2];
    mess_a_traiter[3]=mess_recu[(*indice_courant)+3];
    lon=longueur(mess_a_traiter);
    if (lon<=0) {printf("--message recu de taille incorrecte\n"); return 0;}
    if (lu<((*indice_courant)+lon)) {printf("--message recu de longueur incorrecte\n"); return 0;}
    for (i=4;i<lon;i++) {
      mess_a_traiter[i]=mess_recu[(*indice_courant)+i];
    }
    mess_a_traiter[lon]='\0';
    *indice_courant+=lon;
    return 1;
  } else return 0;
}

void afficher_tout(char* mess, int lu) {
  int i;
  for (i=0;i<lu;i++) {
    printf("%c",mess[i]);
  }
  printf("\n");
}

void afficher(char* mess) {
  int lon, debut,i;
  lon=(mess[0]-'0')*1000 + (mess[1]-'0')*100 + (mess[2]-'0')*10 + (mess[3]-'0') ;
  debut=4;
  for (i=0;i<lon;i++) {
    printf("%c",mess[i]);
  }
  printf("\n");
}

//pour ajouter la longueur d'une chaine sur 4 char
void ajouter_longueur_4(char* msg_temp, char* chaine, int* ind_temp, int* lon) {
  int l;
  l=strlen(chaine);
  msg_temp[(*ind_temp)+3]='0'+l%10;
  msg_temp[(*ind_temp)+2]='0'+(l%100)/10;
  msg_temp[(*ind_temp)+1]='0'+(l%1000)/100;
  msg_temp[(*ind_temp)]='0'+(l%10000)/1000;
  (*ind_temp)+=4;
  (*lon)+=4;
}

//pour ajouter la chaine elle-meme
void ajouter_chaine(char* msg_temp, char* chaine, int* ind_temp, int* lon) {
int i=0;
while (chaine[i]!='\0') {
  msg_temp[*ind_temp]=chaine[i];
  (*ind_temp)++;
  (*lon)++;
  i++;
 }
}

void ajouter_donnees(char* msg_temp, char* donnees, int taille, int* ind_temp, int* lon) {
int i=0;
while (i<taille) {
  msg_temp[*ind_temp]=donnees[i];
  (*ind_temp)++;
  (*lon)++;
  i++;
 }
}

void recopier_longueur(char* res,  int* lon, int* indice_msg ){
   int longueur;
     longueur=(*lon)+4;
     res[3]='0'+longueur%10;
     res[2]='0'+(longueur%100)/10;
     res[1]='0'+(longueur%1000)/100;
     res[0]='0'+(longueur%10000)/1000;
     *indice_msg=4;
}

void ajouter_entier(char* msg_temp,int id,int* ind_temp,int* lon) {
  int l=id;
  msg_temp[(*ind_temp)+3]='0'+l%10;
  msg_temp[(*ind_temp)+2]='0'+(l%100)/10;
  msg_temp[(*ind_temp)+1]='0'+(l%1000)/100;
  msg_temp[(*ind_temp)]='0'+(l%10000)/1000;
  (*ind_temp)+=4;
  (*lon)+=4;
}

void ajouter_entier_8(char* msg_temp,int id,int* ind_temp,int* lon) {
  int l=id;
  msg_temp[(*ind_temp)+7]='0'+l%10;
  msg_temp[(*ind_temp)+6]='0'+(l%100)/10;
  msg_temp[(*ind_temp)+5]='0'+(l%1000)/100;
  msg_temp[(*ind_temp+4)]='0'+(l%10000)/1000;
  msg_temp[(*ind_temp)+3]='0'+ (l%100000)/10000;
  msg_temp[(*ind_temp)+2]='0'+(l%1000000)/100000;
  msg_temp[(*ind_temp)+1]='0'+(l%10000000)/1000000 ;
  msg_temp[(*ind_temp)]='0'+ (l%100000000)/10000000;
  (*ind_temp)+=8;
  (*lon)+=8;
}

void constituer_msg_helo(char* res, char* pseudo,char* tube) {
 char msg_temp[TAILLE_MAX];
 int ind_temp=0,indice_msg=0,i,lon=0;
 msg_temp[0]='H';
 msg_temp[1]='E';
 msg_temp[2]='L';
 msg_temp[3]='O';
 ind_temp=4;
 lon=4;
 ajouter_longueur_4(msg_temp,pseudo,&ind_temp,&lon);
 ajouter_chaine(msg_temp,pseudo,&ind_temp,&lon);
 ajouter_longueur_4(msg_temp,tube,&ind_temp,&lon);
 ajouter_chaine(msg_temp,tube,&ind_temp,&lon);
 recopier_longueur(res, &lon, &indice_msg );
 for (i=0;i<lon;i++) {
   res[indice_msg]=msg_temp[i];
   indice_msg++;
 }
}

void constituer_msg_okok(char* res, int id) {
 char msg_temp[TAILLE_MAX];
 int ind_temp=0,indice_msg=0,i,lon=0;
 msg_temp[0]='O';
 msg_temp[1]='K';
 msg_temp[2]='O';
 msg_temp[3]='K';
 ind_temp=4;
 lon=4;
 ajouter_entier(msg_temp,id,&ind_temp,&lon);
 recopier_longueur(res, &lon, &indice_msg );
 for (i=0;i<lon;i++) {
   res[indice_msg]=msg_temp[i];
   indice_msg++;
 }
}

void constituer_msg_badd(char* res) {
 char msg_temp[TAILLE_MAX];
 int ind_temp=0,indice_msg=0,i,lon=0;
 msg_temp[0]='B';
 msg_temp[1]='A';
 msg_temp[2]='D';
 msg_temp[3]='D';
 ind_temp=4;
 lon=4;
 recopier_longueur(res, &lon, &indice_msg );
 for (i=0;i<lon;i++) {
   res[indice_msg]=msg_temp[i];
   indice_msg++;
 }
}

void constituer_msg_byee(char* res, int id) {
 char msg_temp[TAILLE_MAX];
 int ind_temp=0,indice_msg=0,i,lon=0;
 msg_temp[0]='B';
 msg_temp[1]='Y';
 msg_temp[2]='E';
 msg_temp[3]='E';
 ind_temp=4;
 lon=4;
 ajouter_entier(msg_temp,id,&ind_temp,&lon);
 recopier_longueur(res, &lon, &indice_msg );
 for (i=0;i<lon;i++) {
   res[indice_msg]=msg_temp[i];
   indice_msg++;
 }
}

void constituer_msg_bcst_client(char* res, int id, char* mess) {
 char msg_temp[TAILLE_MAX];
 int ind_temp=0,indice_msg=0,i,lon=0;
 msg_temp[0]='B';
 msg_temp[1]='C';
 msg_temp[2]='S';
 msg_temp[3]='T';
 ind_temp=4;
 lon=4;
 ajouter_entier(msg_temp,id,&ind_temp,&lon);
 ajouter_longueur_4(msg_temp,mess,&ind_temp,&lon);
 ajouter_chaine(msg_temp,mess,&ind_temp,&lon);
 recopier_longueur(res, &lon, &indice_msg );
 for (i=0;i<lon;i++) {
   res[indice_msg]=msg_temp[i];
   indice_msg++;
 }
}

void constituer_msg_bcst_serveur(char* res, char* pseudo,char* mess) {
 char msg_temp[TAILLE_MAX];
 int ind_temp=0,indice_msg=0,i,lon=0;
 msg_temp[0]='B';
 msg_temp[1]='C';
 msg_temp[2]='S';
 msg_temp[3]='T';
 ind_temp=4;
 lon=4;
 ajouter_longueur_4(msg_temp,pseudo,&ind_temp,&lon);
 ajouter_chaine(msg_temp,pseudo,&ind_temp,&lon);
 ajouter_longueur_4(msg_temp,mess,&ind_temp,&lon);
 ajouter_chaine(msg_temp,mess,&ind_temp,&lon);
 recopier_longueur(res, &lon, &indice_msg );
 for (i=0;i<lon;i++) {
   res[indice_msg]=msg_temp[i];
   indice_msg++;
 }
}

void constituer_msg_list_client(char* res, int id) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='L';
  msg_temp[1]='I';
  msg_temp[2]='S';
  msg_temp[3]='T';
  ind_temp=4;
  lon=4;
  ajouter_entier(msg_temp,id,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

void constituer_msg_list_serveur(char* res, int n,char* pseudo) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='L';
  msg_temp[1]='I';
  msg_temp[2]='S';
  msg_temp[3]='T';
  ind_temp=4;
  lon=4;
  ajouter_entier(msg_temp,n,&ind_temp,&lon);
  ajouter_longueur_4(msg_temp,pseudo,&ind_temp,&lon);
  ajouter_chaine(msg_temp,pseudo,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

void constituer_msg_prvt_client(char* res, int id, char* pseudo, char* mess) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='P';
  msg_temp[1]='R';
  msg_temp[2]='V';
  msg_temp[3]='T';
  ind_temp=4;
  lon=4;
  ajouter_entier(msg_temp,id,&ind_temp,&lon);
  ajouter_longueur_4(msg_temp,pseudo,&ind_temp,&lon);
  ajouter_chaine(msg_temp,pseudo,&ind_temp,&lon);
  ajouter_longueur_4(msg_temp,mess,&ind_temp,&lon);
  ajouter_chaine(msg_temp,mess,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

void constituer_msg_prvt_serveur(char* res,char* pseudo, char* mess) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='P';
  msg_temp[1]='R';
  msg_temp[2]='V';
  msg_temp[3]='T';
  ind_temp=4;
  lon=4;
  ajouter_longueur_4(msg_temp,pseudo,&ind_temp,&lon);
  ajouter_chaine(msg_temp,pseudo,&ind_temp,&lon);
  ajouter_longueur_4(msg_temp,mess,&ind_temp,&lon);
  ajouter_chaine(msg_temp,mess,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

void constituer_msg_shut_client(char* res,int id) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='S';
  msg_temp[1]='H';
  msg_temp[2]='U';
  msg_temp[3]='T';
  ind_temp=4;
  lon=4;
  ajouter_entier(msg_temp,id,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

void constituer_msg_shut_serveur(char* res,char* pseudo) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='S';
  msg_temp[1]='H';
  msg_temp[2]='U';
  msg_temp[3]='T';
  ind_temp=4;
  lon=4;
  ajouter_longueur_4(msg_temp,pseudo,&ind_temp,&lon);
  ajouter_chaine(msg_temp,pseudo,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}



void constituer_msg_sendfile_client(char* res,int serie,int id,char* pseudo,int long_fichier,char *nom_de_fichier) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='F';
  msg_temp[1]='I';
  msg_temp[2]='L';
  msg_temp[3]='E';
  ind_temp=4;
  lon=4;
  ajouter_entier(msg_temp,serie,&ind_temp,&lon);
  ajouter_entier(msg_temp,id,&ind_temp,&lon);
  ajouter_longueur_4(msg_temp,pseudo,&ind_temp,&lon);
  ajouter_chaine(msg_temp,pseudo,&ind_temp,&lon);
  ajouter_entier_8(msg_temp,long_fichier,&ind_temp,&lon);

  ajouter_longueur_4(msg_temp,nom_de_fichier,&ind_temp,&lon);
  ajouter_chaine(msg_temp,nom_de_fichier,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

void constituer_msg_senddest_serveur(char* res,int serie,int ind_transfert,int long_fichier,char* nom_de_fichier) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='F';
  msg_temp[1]='I';
  msg_temp[2]='L';
  msg_temp[3]='E';
  ind_temp=4;
  lon=4;
  ajouter_entier(msg_temp,serie,&ind_temp,&lon);
  ajouter_entier(msg_temp,ind_transfert,&ind_temp,&lon);
  ajouter_entier_8(msg_temp,long_fichier,&ind_temp,&lon);
  ajouter_longueur_4(msg_temp,nom_de_fichier,&ind_temp,&lon);
  ajouter_chaine(msg_temp,nom_de_fichier,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

void constituer_msg_sendsrcok_serveur(char* res,int ind_transfert) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='O';
  msg_temp[1]='K';
  msg_temp[2]='O';
  msg_temp[3]='K';
  ind_temp=4;
  lon=4;
  ajouter_entier(msg_temp,ind_transfert,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

void constituer_msg_paquet(char* res,int serie,int ind_transfert,char* donnees,int taille) {
  char msg_temp[TAILLE_MAX];
  int ind_temp=0,indice_msg=0,i,lon=0;
  msg_temp[0]='F';
  msg_temp[1]='I';
  msg_temp[2]='L';
  msg_temp[3]='E';
  ind_temp=4;
  lon=4;
  ajouter_entier(msg_temp,serie,&ind_temp,&lon);
  ajouter_entier(msg_temp,ind_transfert,&ind_temp,&lon);
  ajouter_donnees(msg_temp,donnees,taille,&ind_temp,&lon);
  recopier_longueur(res, &lon, &indice_msg );
  for (i=0;i<lon;i++) {
    res[indice_msg]=msg_temp[i];
    indice_msg++;
  }
}

/*int main(int argc, char* argv[]) {
 char mon_message[TAILLE_MAX];
 char* pseudo="jamesbond djamel chekroun";
 char* tube="/truc/machin/truc2/machin.text";
 constituer_msg_helo(mon_message,pseudo,tube);
 afficher(mon_message);
 constituer_msg_okok(mon_message,18);
 afficher(mon_message);
 constituer_msg_badd(mon_message);
 afficher(mon_message);
 constituer_msg_byee(mon_message,18);
 afficher(mon_message);
 constituer_msg_bcst_client(mon_message,18,"message à tous");
 afficher(mon_message);
 constituer_msg_bcst_serveur(mon_message,"toto","message pour tous");
 afficher(mon_message);
 return 0;

}*/
