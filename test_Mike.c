#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header_Mike.h"



int main() {
    graphe *g = lireFichier("exclusions.txt");
    
    welshPowell(g,colorations);
    printf("Tableau de Colorations :\n");
    int index=1;
    for (size_t i = 0; i < g->taille; i++) {

        if(colorations->Couleurs[i][0]!=0&&colorations->Couleurs[i][0]<=g->taille){
            printf("Couleur %zu : [", index );
            index++;

            for (int j = 0; j < g->taille; j++) {
                if(colorations->Couleurs[i][j]!=0&&colorations->Couleurs[i][j]<=g->taille){
                    printf("%d,", colorations->Couleurs[i][j]);
                }


            }

            printf("]\n");}
    }

    for (int i = 0; i < g->taille; i++) {
        free(colorations->Couleurs[i]);
    }
    free(colorations->Couleurs);
    free(colorations);

    free(g->tabOperations);
    for (int i = 1; i <= g->taille; i++) {
        free(g->listeArc[i].adjacents);
    }
    free(g->listeArc);

    return 0;
}