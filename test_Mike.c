#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header_Mike.h"
graphe *lireFichier(const char *nomFichier);
int *TriParDegreGraphe(graphe *g);
Colorations *Coloration(graphe *g);
void welshPowell(graphe *g, Colorations *colorations);


int main() {
    graphe *g = lireFichier("exclusions.txt");
    Colorations *colorations= malloc(sizeof(Colorations));
    colorations->Couleurs=malloc(g->taille*sizeof(int*));
    for(int b=0;b<g->taille;b++){
        colorations->Couleurs[b]=malloc(sizeof(int)*g->taille);
    }
    welshPowell(g,colorations);
    printf("Nombre total de couleurs utilisées : %d\n", colorations->Couleurs[0][0]);

    for (int i = 1; i <= colorations->Couleurs[0][0]; i++) {
        printf("Couleur %d : ", i);
        for (int j = 1; j < g->taille; j++) {
            if (colorations->Couleurs[j][0] == i) {
                printf("%d ", j);
            }
        }
        printf("\n");
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
