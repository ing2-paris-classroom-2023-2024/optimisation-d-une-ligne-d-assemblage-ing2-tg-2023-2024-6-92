#include "header.h"


int main(void){
    operations_l * liste_operation;

    float temps_cycle = lecture_temps_cycle();
    

    int nb_operation = lecture_fichier_operation(&liste_operation);
    operations_l ** liste_operations = malloc(sizeof(operations_l*)*nb_operation);

    for(int i = 0;i<nb_operation;i++){
        liste_operations[i] = &liste_operation[i];
    }

    //float chemin_plus_rapide = calcul_chemin_rapide(&liste_operations, 10);
    float autre_chemin = calcul_chemin_possible_rapide(&liste_operations,nb_operation,0);
    //printf("Chemin plus rapide %f \n",chemin_plus_rapide);
    printf("Chemin plus rapide %f \n",autre_chemin);
    
    /*
    precedence_init(nb_operation,&liste_operation);
    for(int i = 0;i<nb_operation;i++)
        printf("Operation : %i, temps : %0.2f\n",liste_operation[i].operation,liste_operation[i].temps);
    
    for(int i = 0;i<nb_operation;i++){
        for(int j = 0;j<liste_operation[i].nb_operation_precedente;j++){
            printf("%i doit se passer avant la tache %i\n",liste_operation[i].op_precedent[j],liste_operation[i].operation);
        }
    }
        
    printf("Il y a %i operation.\n",nb_operation);

    printf("Il y a %0.2f temps cycle.\n",temps_cycle);
    */
    return 0;
}


