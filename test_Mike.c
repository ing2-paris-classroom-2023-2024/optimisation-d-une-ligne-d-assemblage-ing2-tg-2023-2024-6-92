#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header_Mike.h"
graphe *lireFichier(const char *nomFichier);
int *TriParDegreGraphe(graphe *g);
bool estAdj(graphe *g,int s1,int s2);
bool estAdjTab(graphe *g, int *tab, int sommet, size_t index);
void welshPowell(graphe *g, Colorations *colorations);


int main() {
    graphe *g = lireFichier("exclusions.txt");
    Colorations *colorations= malloc(sizeof(Colorations));
    colorations->Couleurs=malloc(g->taille*sizeof(int*));
    for(int b=0;b<g->taille;b++){
        colorations->Couleurs[b]=malloc(sizeof(int)*g->taille);
    }
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