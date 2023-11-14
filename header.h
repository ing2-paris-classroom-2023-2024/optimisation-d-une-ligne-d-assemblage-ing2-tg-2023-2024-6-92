#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int operation;
    float temps;
}operations_l;

int lecture_fichier_operation(operations_l* liste_operation);

// Ouverture du fichier operation pour lecture des valeurs et inclusions dans une liste operation_l 
int lecture_fichier_operation(operations_l* liste_operation){
    FILE* fichier_ope;
    fichier_ope = fopen("operation.txt","r");
    if(fichier_ope==NULL){
        printf("Erreur lors de l'ouverture du fichier d'operation\n");
        return -1;
    }
    int operation_acutelle;
    float temps_actuelle;
    int nombre_operation = 0;
    while(fscanf(fichier_ope,"%i %f",&operation_acutelle,&temps_actuelle)){
        nombre_operation++;
        realloc(liste_operation,sizeof(operations_l)*nombre_operation);     // permet de gérer le fait qu'on ne connaise pas le nombre d'opération à l'avance 
        liste_operation[nombre_operation-1].operation = operation_acutelle;
        liste_operation[nombre_operation-1].temps = temps_actuelle;
    }
    return nombre_operation;
}