#include "header.h"


int main(void){
    operations_l * liste_operation;

    float temps_cycle = lecture_temps_cycle();
    

    int nb_operation = lecture_fichier_operation(&liste_operation);

    
    precedence_init(nb_operation,&liste_operation);
    implementation_Pert(&liste_operation,nb_operation);
    
    
    return 0;
}


