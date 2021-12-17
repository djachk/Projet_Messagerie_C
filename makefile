
all: tchatche_serveur tchatche

tchatche:client.o constituer_msg_tous.o utilitaires.o traiter_messages_client.o lire_msg_tous.o traiter_commandes_client.o
	gcc -g -o tchatche  client.o constituer_msg_tous.o utilitaires.o traiter_messages_client.o lire_msg_tous.o traiter_commandes_client.o

traiter_messages_client.o: traiter_messages_client.c traiter_messages_client.h constituer_msg_tous.h donnees_client.h utilitaires.h
	gcc -g -o traiter_messages_client.o -c traiter_messages_client.c

traiter_commandes_client.o: traiter_commandes_client.c traiter_commandes_client.h donnees_client.h utilitaires.h constituer_msg_tous.h
	gcc -o traiter_commandes_client.o -c traiter_commandes_client.c

client.o:client.c utilitaires.h constituer_msg_tous.h constantes.h traiter_messages_client.h traiter_commandes_client.h donnees_client.h
	gcc -g -o client.o -c client.c


tchatche_serveur:serveur.o constituer_msg_tous.o traiter_messages_serveur.o lire_msg_tous.o utilitaires.o
	gcc -g -o tchatche_serveur  serveur.o constituer_msg_tous.o traiter_messages_serveur.o lire_msg_tous.o utilitaires.o

constituer_msg_tous.o:constituer_msg_tous.c utilitaires.h constantes.h
	gcc -g -o constituer_msg_tous.o -c constituer_msg_tous.c

traiter_messages_serveur.o: traiter_messages_serveur.c lire_msg_tous.h traiter_messages_serveur.h utilitaires.h constituer_msg_tous.h constantes.h
	gcc -g -o traiter_messages_serveur.o -c traiter_messages_serveur.c

utilitaires.o: utilitaires.c utilitaires.h
	gcc -g -o utilitaires.o -c utilitaires.c;

lire_msg_tous.o: lire_msg_tous.c constituer_msg_tous.h constantes.h utilitaires.h lire_msg_tous.h
	gcc -g -o lire_msg_tous.o -c lire_msg_tous.c

serveur.o:serveur.c utilitaires.h constituer_msg_tous.h constantes.h traiter_messages_serveur.h
	gcc -g -o serveur.o -c serveur.c

clean:
	rm *.o serveur client tchatche tchatche_serveur
