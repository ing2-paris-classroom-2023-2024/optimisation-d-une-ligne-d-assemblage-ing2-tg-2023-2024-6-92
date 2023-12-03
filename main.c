#include "header.h"
void affichageStations( graphe* g,operations_l** liste_operations,int nb_operations);

int main(void){
    graphe *g = lireFichier("exclusions.txt");
    operations_l * liste_operation;

    int temps_cycle = lecture_temps_cycle();
    printf("TEMPS CYCLE = %i\n \n",temps_cycle);

    int nb_operation = lecture_fichier_operation(&liste_operation);

    welshPowell(g,&liste_operation);
    affichageStations(g,&liste_operation,nb_operation);
    for(int i = 0; i<g->taille;i++){
        for(int j = 0;j<nb_operation;j++){
            if(g->listeArc->nom==liste_operation[j].operation)
                liste_operation[j].couleur = g->listeArc->couleur;
        }
    }
    precedence_init(nb_operation,&liste_operation);
    implementation_Pert(liste_operation,nb_operation,temps_cycle);
    
    
    return 0;
}

void affichageStations( graphe* g,operations_l** liste_operations,int nb_operations) {
    printf("\n \nAFFICHAGE DES STATIONS\n");
    for (int i = 0; i < g->nbStations; i++) {

            printf("\n Station %d\n", i);
            printf("\n------------------------------------------------------------------------------------------------------------------------\n");
            printf("| ");
            for (int j = 0; j < g->taille; j++) {
                if (g->listeArc[j].couleur==i) {
                    printf("op %d,  ", j);
                }
            }
            printf("|\n------------------------------------------------------------------------------------------------------------------------\n");
        }
}


