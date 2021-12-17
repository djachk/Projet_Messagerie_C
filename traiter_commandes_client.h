#ifndef _TRAITER_COMMANDES_CLIENT_H
#define _TRAITER_COMMNDES_CLIENT_H
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>


void traiter_commande_client(char* mess_a_traiter);

#endif
