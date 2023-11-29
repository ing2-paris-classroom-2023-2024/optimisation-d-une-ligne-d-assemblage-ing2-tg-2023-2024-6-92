#include <stdio.h>
#include <stdlib.h>

#define True 1
#define False 0


typedef struct {
    int operation;
    float temps;
    int* op_precedent;
    int nb_operation_precedente;
    int effectuer;
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
        (*liste_operation)[operation_actuelle-1].effectuer = False;
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


void pert_algo(int nombre_operations, operations_l **liste_operations,float temps_cycle){ // ici on va essayer d'implementer la methode de Pert qui ressemble beaucoup à notre cas d'usage
    int** liste_operations_effectuer = (int*) malloc(sizeof(int)*nombre_operations);     //liste des operations qui ont deja ete effectue par cycle, quand i = 0 alors c'est le premier cycle toutes les operations effectue
    for(int i = 0;i<nombre_operations;i++){
        liste_operations_effectuer[i] = (int*) malloc(sizeof(int)*nombre_operations);
    }
    int * operations_effectuer = (int*) malloc(sizeof(int)*nombre_operations);           // cette liste nous permet d'acceder plus rapidement et plus facilement a toutes les operations effectuer sans avoir a faire un balayge en 2D
    int* operations_facile = (int*) malloc(sizeof(int) * nombre_operations);            //liste des operations consideré comme facile à effectue car elle ne requiert aucunes autre tache pour etre effectue
    int operations_trie_precedence = (int) malloc(sizeof(int)*nombre_operations);     // liste qui contient les operations trie par ordre croissant de nombre de precedances, plus une operation à besoin de precedences plus elle sera mise vers le fin de la liste
    int* operations_possible = (int*) malloc(sizeof(int)*nombre_operations);            //liste des operations pouvants etre effectue à un instant T
    int operations_facile_index = -1;
    for(int operation_actuelle=0;operation_actuelle<nombre_operations;operation_actuelle++){
        if((*liste_operations)[operation_actuelle].nb_operation_precedente==0)
            operations_facile[operations_facile_index++] = (*liste_operations)[operation_actuelle].operation;
    }
}

void tri_ordre_operation(tuple** operations_trie, operations_l** liste_operations,int nombre_operations){
    tuple provisoire;
    for(int i = 0;i<nombre_operations;i++){
        (*operations_trie)[i].sommet = (*liste_operations)[i].operation;
        (*operations_trie)[i].nb = (*liste_operations)[i].nb_operation_precedente;
    }
    for(int i = 0; i<nombre_operations;i++){
        for(int j = 0;j<nombre_operations;j++){
            if((*operations_trie)[j].nb>(*operations_trie)[j+1].nb){
                provisoire.nb = (*operations_trie)[j].nb;
                provisoire.sommet = (*operations_trie)[j].sommet;
                (*operations_trie)[j].nb = (*operations_trie)[j+1].nb;
                (*operations_trie)[j].sommet = (*operations_trie)[j+1].sommet;
                (*operations_trie)[j+1].nb = provisoire.nb;
                (*operations_trie)[j+1].sommet = provisoire.sommet;

            }
        }
    }
}



void update_operations_possible(int index_operation_facile,int **operation_possible, int nombre_operation, int * operation_effectuer, int * opeartions_facile, operations_l **liste_operation,float temps_cycle){  //cette fonction va nous permettre de mettre a jour notre liste contenant les operations effectuables à un instant T
    int nombre_operation_possible = 0;
    int possible = 1;
    for(int i = 0;i<index_operation_facile;i++){
        (*operation_possible)[i] = opeartions_facile[i];
        nombre_operation_possible++;
    }
    if(calcul_operations_temps(*operation_possible,liste_operation,nombre_operation,nombre_operation_possible,temps_cycle)==1)
        return;
    for(int operation_i = 0;operation_i<nombre_operation;operation_i++){
        for(int operation_j=0;operation_j<(*liste_operation)[operation_i].nb_operation_precedente;operation_j++){
            possible = 1;
            for(int operation_effectue = 0; operation_effectue < sizeof(operation_effectuer)/sizeof(int);operation_effectue++){            // ca ne fonctionne pas mais c'est un debut 
                if((*liste_operation)[operation_i].op_precedent[operation_j] != operation_effectuer[operation_effectue]){
                    possible=1;
                }
            }
            if(possible==1){
                (*operation_possible)[nombre_operation_possible] = (*liste_operation)[operation_i].operation;
                nombre_operation_possible++;
            }
        }
    }
    return;
}

int calcul_operations_temps(int* liste,operations_l** liste_operations,int nb_operations, int nombre_operations_possible,float temps_cyle){
    float time = 0;
    for(int i = 0;i<nombre_operations_possible;i++){
        for(int j = 0;j<nb_operations;j++){
            if(j = liste[i])
                time+= (float) (*liste_operations)[j].temps;
        }
    }
    if(time> temps_cyle)
        return 1;
    return 0;
}



void calcul_chemin_rapide(operations_l** liste_operations, operations_l** operation_effectuable)