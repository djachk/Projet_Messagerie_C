#ifndef _LIRE_MSG_TOUS_H
#define _LIRE_MSG_TOUS_H

int lire_msg_helo(char* res, char* pseudo,char* tube);
int lire_msg_okok(char* res, int* id) ;
int lire_msg_bcst_serveur(char* res, int* id, char* mess);
int lire_msg_bcst_client(char* res, char* pseudo, char* mess);
int lire_msg_list_serveur(char* res, int* id);
int lire_msg_list_client(char* res, int* n, char* pseudo);
int lire_msg_byee_serveur(char* res, int* id);
int lire_msg_prvt_serveur(char* res, int* id, char* pseudo, char* mess);
int lire_msg_prvt_client(char* res, char* pseudo, char* mess);
int lire_msg_shut_serveur(char* res, int* id);
int lire_msg_shut_client(char* res, char* pseudo);
int lire_msg_sendfile_serveur(char* res, int* serie,int* id_client, char* pseudo_dest,int* long_fichier,char* nom_de_fichier);
int lire_msg_sendfile_client(char* res, int* serie,int* id_transfert, int* long_fichier,char* nom_de_fichier);
int lire_msg_paquet(char* res,int* serie,int* ind_transfert,char* donnees,int* taille);


/*void lire_msg_badd(char* res);
void lire_msg_byee(char* res, int id);


*/

#endif
