#include "header.h"


int main(void){
    operations_l * liste_operation;

    float temps_cycle = lecture_temps_cycle();
    
    
    int nb_operation = lecture_fichier_operation(&liste_operation);
    
    
    for(int i = 0;i<nb_operation;i++)
        printf("Operation : %i, temps : %0.2f\n",liste_operation[i].operation,liste_operation[i].temps);
    printf("Il y a %i operation.\n",nb_operation);

    printf("Il y a %0.2f temps cycle.\n",temps_cycle);
    return 0;
}