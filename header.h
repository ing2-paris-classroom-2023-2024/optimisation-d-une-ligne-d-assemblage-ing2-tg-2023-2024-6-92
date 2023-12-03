#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False 0


typedef struct {
    int operation;                          // ID DE OPERATION
    float temps;                            // TEMPS NECESSAIRE POUR EFFECTUER OPERATION
    int* op_precedent;                      // LISTE DU NOMBRE OPERATION PRECEDENTE
    int nb_operation_precedente;            // INDIQUE LE NOMBRE OPERATION PRECEDENTE NECCASSAIRE POUR EFFECTUER OPERATION
    int effectuer;                          // INDIQUE SI OPERATION A ETE EFFECTUER, ELLE EST DEJA DANS UNE STATION
    int effectuable;                        // INDIQUE SI OPERATION EST EFFECTUABLE, TOUTES LES OPERATIONS PRECEDENTES SONT DEJA EFFECTUER
    int couleur;
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

typedef struct 
{
    int nb_operation_max;
    int nb_operation_actuelle;
    operations_l * liste_operation;
    float temps_total;
}station;




int lecture_fichier_operation(operations_l** liste_operation);
float lecture_temps_cycle(void);
int nb_ligne_fichier(char* nom_fichier);
void precedence_init(int nombre_operation,operations_l** liste_operations);

int check_operation_possible(operations_l ** liste_operation,int nb_operation);
int comptage_operation(operations_l** liste_operation,int nb_operation);
void implementation_Pert(operations_l** liste_operation, int nombre_operation);
void associe_liste_operation_possible(operations_l** liste_operation,operations_l *** liste_operation_possible,int nb_operation);

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
        (*liste_operation)[operation_actuelle-1].effectuer = False;
        (*liste_operation)[operation_actuelle-1].effectuable = False;
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
    while(fscanf(fichier_prece,"%i %i\n",&liste_precedent[actuelle].op2,&liste_precedent[actuelle].op1)==2)
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
        if(nb_operation_precedentes[i]==0)
            (*liste_operations)[i].effectuable = True;
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

/*
float calcul_chemin_rapide(operations_l*** operation_effectuable, int nb_operation_effectuable){
    //int nb_operation_effectuable = (int) (sizeof(*operation_effectuable)/sizeof(operations_l*));
    if(nb_operation_effectuable==1)
        return (*operation_effectuable)[0]->temps;
    float* chemins_possible = (float*) malloc(sizeof(int)*nb_operation_effectuable);
    float chemin_actuel;
    
    for(int operation_i = 0; operation_i<nb_operation_effectuable;operation_i++){
        int index = 0;
        operations_l** nouvelles_liste = (operations_l**) malloc(sizeof(operations_l*)*(nb_operation_effectuable-1));
        for(int operation_j = 0;operation_j<nb_operation_effectuable;operation_j++){
            if(operation_j!=operation_i){
                nouvelles_liste[index] = (*operation_effectuable)[operation_j];
                index++;
            }
        }
        chemin_actuel = calcul_chemin_rapide(&nouvelles_liste,nb_operation_effectuable-1);
        if(chemin_actuel+((*operation_effectuable)[operation_i])->temps<=10)
            chemins_possible[operation_i] = ((*operation_effectuable)[operation_i])->temps + chemin_actuel;
        else
            chemins_possible[operation_i] = chemin_actuel;
        free(nouvelles_liste);
    }
    float plus_rapide = 0;
    for(int chemins = 0; chemins < nb_operation_effectuable; chemins++){
        if(chemins_possible[chemins]>plus_rapide)
            plus_rapide = chemins_possible[chemins];
    }
    free(chemins_possible);
    return plus_rapide;
}
*/
// CETTE FONCTION EST OPERATIONELLE MAIS NECESSITE UNE GRANDE PUISSANCE DE CALCUL, CEPENDANT ELLE PEUT DONNER DES RESULTATS PLUS SATISFAISANT


float calcul_chemin_possible_rapide(operations_l*** operation_effectuable, int nb_operation_effectuable,float temps_actuel, station *station_t){
    float valeur_final = 0;
    float plus_grand = 0;
    int index_plus_grand =-1;
    if(nb_operation_effectuable ==0)
        return 0;
    for(int operation_i = 0; operation_i<nb_operation_effectuable;operation_i++){
        if((*operation_effectuable)[operation_i]->temps > plus_grand && (*operation_effectuable)[operation_i]->temps + temps_actuel <= 10){
            plus_grand = (*operation_effectuable)[operation_i]->temps;
            index_plus_grand = operation_i;
        }
    }
    if(index_plus_grand == -1)
        return 0;
    int index = 0;
    operations_l** nouvelles_liste = (operations_l**) malloc(sizeof(operations_l*)*(nb_operation_effectuable-1));
    for(int operation_j = 0;operation_j<nb_operation_effectuable;operation_j++){
        if(operation_j!=index_plus_grand){
            nouvelles_liste[index] = (*operation_effectuable)[operation_j];
            index++;
        }
    }
    (*operation_effectuable)[index_plus_grand]->effectuer = True;
    station_t->liste_operation[station_t->nb_operation_actuelle] = *((*operation_effectuable)[index_plus_grand]);
    station_t->nb_operation_actuelle++;
    valeur_final = (*operation_effectuable)[index_plus_grand]->temps + calcul_chemin_possible_rapide(&nouvelles_liste,index,temps_actuel+(*operation_effectuable)[index_plus_grand]->temps,station_t);

    return valeur_final;
}




void implementation_Pert(operations_l** liste_operation, int nombre_operation){
    int nb_operation_restante = comptage_operation(liste_operation,nombre_operation);
    int nb_operation_possible = 0;
    int index_station = 0;
    station *liste_station = malloc(sizeof(station)*nombre_operation);
    while(nb_operation_restante!=0){
        nb_operation_possible = check_operation_possible(liste_operation,nombre_operation);
        operations_l **liste_operation_possible = (operations_l**) malloc(sizeof(operations_l*)*nb_operation_possible);
        associe_liste_operation_possible(liste_operation,&liste_operation_possible,nombre_operation);
        liste_station[index_station].nb_operation_max = nb_operation_possible;
        liste_station[index_station].nb_operation_actuelle = 0;
        liste_station[index_station].temps_total = 0;
        liste_station[index_station].liste_operation = malloc(sizeof(operations_l)*nb_operation_possible);
        calcul_chemin_possible_rapide(&liste_operation_possible,nb_operation_possible,0,&(liste_station[index_station]));
        nb_operation_restante = comptage_operation(liste_operation,nombre_operation);
        free(liste_operation_possible);
        index_station++;
    }
}

void associe_liste_operation_possible(operations_l** liste_operation,operations_l *** liste_operation_possible,int nb_operation){
    int index = 0;
    for(int operation_i=0;operation_i<nb_operation;operation_i++){
        if((*liste_operation)[operation_i].effectuable == True && (*liste_operation)[operation_i].effectuer == False){
            (*liste_operation_possible)[index] = &((*liste_operation)[operation_i]);
            index++;
            
        }
    }
}




int check_operation_possible(operations_l ** liste_operation,int nb_operation){    // PARCOURS LA LISTE DES OPERATIONS AFIN DE METTRE A JOUR LEURS ETATS
    int actuelle_etat;
    int nb_operation_effectuable= 0;
    for(int operation_i = 0;operation_i<nb_operation;operation_i++){
        actuelle_etat = False;
        if((*liste_operation)[operation_i].effectuer==False){
            actuelle_etat = True;
            for(int op_precedente_i = 0;op_precedente_i<(*liste_operation)[operation_i].nb_operation_precedente;op_precedente_i++){
                for(int operation_j = 0;operation_j<nb_operation;operation_j++){
                    if((*liste_operation)[operation_i].op_precedent[op_precedente_i]==(*liste_operation)[operation_j].operation){
                        if((*liste_operation)[operation_j].effectuer==False)
                            actuelle_etat = False;
                    }
                }
            }
            if(actuelle_etat==True){
                (*liste_operation)[operation_i].effectuable = True;
                nb_operation_effectuable++;
            }

        }
    }
    return nb_operation_effectuable;
}

int comptage_operation(operations_l** liste_operation,int nb_operation){    // COMPTE LE NOMBRE OPERATION RESTANTE, LES OPERATIONS QUI N ONT PAS ENCORE ETE EXECUTE
    int nb_operation_restante= 0;
    for(int operation_i = 0;operation_i<nb_operation;operation_i++){        // POUR CHAQUE OPERATION, ON REGARDE SI ELLE A DEJA ETE EXECUTE OU NON
        if((*liste_operation)[operation_i].effectuer==False)
            nb_operation_restante++;
        //printf("Etat : %i",(*liste_operation)[operation_i].effectuer);
    }

    return nb_operation_restante;
}


/*
void nouvelle_liste_tache(operations_l**** operation_effectuable,int nb_operation_effectuable){
    operations_l *** nouvelle_liste = (operations_l***) malloc(sizeof(operations_l*)*(nb_operation_effectuable-1));
    for(int i = 1;i<nb_operation_effectuable;i++){
        (*nouvelle_liste)[i] = (**operation_effectuable)[i]; 
    }
    free(operation_effectuable);
    operation_effectuable = (&nouvelle_liste);
}
*/