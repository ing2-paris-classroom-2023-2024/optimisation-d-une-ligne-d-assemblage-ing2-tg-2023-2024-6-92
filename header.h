#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int operation;
    float temps;
    int* op_precedent;
    int nb_operation_precedente;
}operations_l;

typedef struct
{
    int op1;
    int op2;
}precedent;

typedef struct 
{
    int sommet;
    int nb;
}tuple;



int lecture_fichier_operation(operations_l** liste_operation);
float lecture_temps_cycle(void);
int nb_ligne_fichier(char* nom_fichier);
void precedence_init(int nombre_operation,operations_l** liste_operations);

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
    int nombre_operation = nb_ligne_fichier("operation.txt");  // permet de gérer le fait qu'on ne connaise pas le nombre d'opération à l'avance
    (*liste_operation) = malloc(sizeof(operations_l)*nombre_operation);

    while(fscanf(fichier_ope,"%i %f\n",&operation_acutelle,&temps_actuelle)==2){

        operation_actuelle++;
        (*liste_operation)[operation_actuelle-1].operation = operation_acutelle;
        (*liste_operation)[operation_actuelle-1].temps = temps_actuelle;
        operation_acutelle = 0;
        temps_actuelle = 0;

    }

    fclose(fichier_ope);
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
    fclose(fichier_temps);

    return temps_cycle;  
}

int nb_ligne_fichier(char* nom_fcihier){
    FILE* fichier;
    fichier = fopen(nom_fcihier,"r");
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

void precedence_init(int nombre_operation,operations_l** liste_operations){
    int nb_arc= nb_ligne_fichier("precedences.txt");
    FILE *fichier_prece;
    fichier_prece = fopen("precedences.txt","r");
    if(fichier_prece==NULL){
        printf("Erreur lors de l'ouverture du fichier des precedences.");
        return;
    }

    precedent * liste_precedent = (precedent*) malloc(sizeof(precedent)* nb_arc);
    int actuelle = 0;
    while(fscanf(fichier_prece,"%i %i\n",&liste_precedent[actuelle].op1,&liste_precedent[actuelle].op2)==2)
        actuelle++;

    int **liste_operation_precedente = (int**) malloc(sizeof(int*)*nombre_operation); 
    for(int i = 0;i<nombre_operation;i++){
        liste_operation_precedente[i] = (int*) malloc(sizeof(int)*nombre_operation);
    }
    int *nb_operation_precedentes = (int*) malloc(sizeof(int)*nombre_operation);
    for(int i = 0;i<nombre_operation;i++){
        nb_operation_precedentes[i] = 0;
    }
    int operation_precedente_actuelle = 0;
    for(int i = 0;i<nombre_operation;i++){
        operation_precedente_actuelle = 0;
        for(int j = 0;j<nombre_operation;j++){
            if(liste_precedent[j].op1 ==(*liste_operations)[i].operation){
                liste_operation_precedente[i][operation_precedente_actuelle] = liste_precedent[j].op2;
                operation_precedente_actuelle++;
                nb_operation_precedentes[i]++;
            }
        }
    }

    for(int i = 0;i<nombre_operation;i++){
        (*liste_operations)[i].nb_operation_precedente = nb_operation_precedentes[i];
        (*liste_operations)[i].op_precedent = malloc(sizeof(int)*nb_operation_precedentes[i]);
        for(int j = 0;j<nb_operation_precedentes[i];j++){
            (*liste_operations)[i].op_precedent[j] = liste_operation_precedente[i][j];
        }
    }

    fclose(fichier_prece);
    free(liste_precedent);
    for(int i = 0;i<nombre_operation;i++){
        free(liste_operation_precedente[i]);
    }
    free(liste_operation_precedente);
    free(nb_operation_precedentes);
}


