#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constantes.h"
#include "utilitaires.h"


type_commande type_message(char* mess) {
type_commande le_type_c;
if (mess[4]=='H' && mess[5]=='E'&& mess[6]=='L'&& mess[7]=='O')
  le_type_c=HELO;
else if (mess[4]=='O' && mess[5]=='K'&& mess[6]=='O'&& mess[7]=='K')
  le_type_c=OKOK;
else if (mess[4]=='B' && mess[5]=='A'&& mess[6]=='D'&& mess[7]=='D')
    le_type_c=BADD;
else if (mess[4]=='B' && mess[5]=='Y'&& mess[6]=='E'&& mess[7]=='E')
    le_type_c=BYEE;
else if (mess[4]=='B' && mess[5]=='C'&& mess[6]=='S'&& mess[7]=='T')
    le_type_c=BCST;
else if (mess[4]=='L' && mess[5]=='I'&& mess[6]=='S'&& mess[7]=='T')
    le_type_c=LIST;
else if (mess[4]=='P' && mess[5]=='R'&& mess[6]=='V'&& mess[7]=='T')
    le_type_c=PRVT;
else if (mess[4]=='F' && mess[5]=='I'&& mess[6]=='L'&& mess[7]=='E')
    le_type_c=FILEP;
else if (mess[4]=='S' && mess[5]=='H'&& mess[6]=='U'&& mess[7]=='T')
    le_type_c=SHUT;
else le_type_c=NONE;

return le_type_c;
}

void vider_buffer() {
    char c;
    while(c!='\n' && c!=EOF) {c=getchar();}
}

int lire(char* chaine, int longueur) {
  char* positionEntree=NULL;
if (fgets(chaine,longueur,stdin)!=NULL) {
  positionEntree=strchr(chaine,'\n');
  if (positionEntree!=NULL) {*positionEntree='\0';} else {vider_buffer();}
  return 1;
}
else
{
  vider_buffer();
  return -1;
}
}
