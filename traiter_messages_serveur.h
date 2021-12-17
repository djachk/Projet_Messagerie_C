#ifndef _TRAITER_MESSAGES_SERVEUR_H
#define _TRAITER_MESSAGES_SERVEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>



void initialiser_clients();
void initialiser_transferts();
//void traiter_serveur_helo(char* mess);
void traiter_message_serveur(char* mess_a_traiter);

#endif
