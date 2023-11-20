#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int operation;
    float temps;
}operations_l;

int lecture_fichier_operation(operations_l** liste_operation);
float lecture_temps_cycle(void);
int nb_operation(void);

// Ouverture du fichier operation pour lecture des valeurs et inclusions dans une liste operation_l 
int lecture_fichier_operation(operations_l** liste_operation){
    FILE* fichier_ope;
    fichier_ope = fopen("operation.txt","r");
    if(fichier_ope==NULL){
        printf("Erreur lors de l'ouverture du fichier d'operation\n");
        return -1;
    }
    int operation_acutelle=0;
    float temps_actuelle=0;
    int operation_actuelle= 0;
    int nombre_operation = nb_operation();  // permet de gérer le fait qu'on ne connaise pas le nombre d'opération à l'avance
    (*liste_operation) = malloc(sizeof(operations_l)*nombre_operation);
    while(fscanf(fichier_ope,"%i %f\n",&operation_acutelle,&temps_actuelle)==2){
        operation_actuelle++;
        (*liste_operation)[operation_actuelle-1].operation = operation_acutelle;
        (*liste_operation)[operation_actuelle-1].temps = temps_actuelle;
        operation_acutelle = 0;
        temps_actuelle =0;
    }
    //fclose(fichier_ope);
    return nombre_operation;
}

float lecture_temps_cycle(void){
    FILE* fichier_temps;
    fichier_temps = fopen("temps_cycle.txt","r");
    if(fichier_temps==NULL){
        printf("Erreur lors de la lecture du fichier de temps de cycle");
        return -1;
    }
    float temps_cycle=0;
    fscanf(fichier_temps,"%f",&temps_cycle);
    //fclose(fichier_temps);
    return temps_cycle;  
}

int nb_operation(void){
    FILE* fichier;
    fichier = fopen("operation.txt","r");
    int nombre= 0;
    char c;
    while((c=fgetc(fichier))!=EOF){
        if(c=='\n')
            nombre++;
    }
    nombre++;
    fclose(fichier);
    return nombre;

}