#include "header.h"


int main(void){
    printf("oui\n");
    operations_l * liste_operation;

    int nb_operation = lecture_fichier_operation(liste_operation);

    for(int i = 0;i<nb_operation;i++)
        printf("Operation : %i, temps : %f\n",liste_operation[i].operation,liste_operation[i].temps);
    
    return 0;
}