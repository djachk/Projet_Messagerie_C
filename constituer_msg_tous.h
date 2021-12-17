#ifndef _CONSTITUER_MSG_TOUS_H
#define _CONSTITUER_MSG_TOUS_H

void constituer_msg_helo(char* res, char* pseudo,char* tube);
void constituer_msg_okok(char* res, int id) ;
void constituer_msg_badd(char* res);
void constituer_msg_byee(char* res, int id);
void constituer_msg_bcst_client(char* res, int id, char* mess);
void constituer_msg_bcst_serveur(char* res, char* pseudo,char* mess);
void constituer_msg_list_client(char* res, int id);
void constituer_msg_list_serveur(char* res, int n,char* pseudo);
void constituer_msg_prvt_client(char* res,int id, char* pseudo, char* mess);
void constituer_msg_prvt_serveur(char* res,char* pseudo, char* mess);
void constituer_msg_shut_client(char* res,int id);
void constituer_msg_shut_serveur(char* res,char* pseudo);
void constituer_msg_sendfile_client(char* res,int serie,int id,char* pseudo,int long_fichier,char *nom_de_fichier);
void constituer_msg_senddest_serveur(char* res,int serie,int ind_transfert,int long_fichier,char* nom_de_fichier);
void constituer_msg_sendsrcok_serveur(char* res,int ind_transfert);
void constituer_msg_paquet(char* res,int serie,int ind_transfert,char* donnees, int taille);

void afficher(char* mess);
void afficher_tout(char* mess,int lu);
int longueur(char* mess);
int longueur_8(char* mess);
int lire_serie(char* mess);
int lecture_messages(char* mess_recu, int lu,char *mess_a_traiter,int* indice_courant);

#endif
