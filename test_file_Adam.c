#include "header.h"


int main(void){
    graphe *g = lireFichier("exclusions.txt");
    operations_l * liste_operation;

    int temps_cycle = lecture_temps_cycle();
    printf("TEMPS CYCLE = %i\n \n",temps_cycle);

    int nb_operation = lecture_fichier_operation(&liste_operation);

    welshPowell(g,&liste_operation);
    
    precedence_init(nb_operation,&liste_operation);
    implementation_Pert(&liste_operation,nb_operation,temps_cycle);
    
    
    return 0;
}


