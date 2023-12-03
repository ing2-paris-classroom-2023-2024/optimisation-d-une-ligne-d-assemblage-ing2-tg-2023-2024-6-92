#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

typedef struct sommet {//structure d'un sommet avec la liste des sommets adjacents et son degré
    int nom;
    int *adjacents;
    int degre;
    int couleur;
} sommetExclu;

typedef struct graphe {//structure du graphe qui comprends la liste des sommets dans l'ordre décroissant des degrés, tous les sommets, la taille et le degré maximum
    int *tabOperations;
    sommetExclu *listeArc;
    int taille;
    int degMax;
    int nbStations;
} graphe;



graphe *lireFichier(const char *nomFichier) {//Fonction qui permet de lire le fichier de contrainte d'exclusions
    //suite à des problèmes d'allocations dynamique nous avons décidé de lire plusieurs fois le fichier
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    int maxNom = 0;
    int maxDegre = 0;
    int sommet1, sommet2;

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//première lecture pour avoir le degré max
        if (sommet1 > maxNom) {
            maxNom = sommet1;
        }
        if (sommet2 > maxNom) {
            maxNom = sommet2;
        }
        if (sommet1 > maxDegre) {
            maxDegre = sommet1;
        }
        if (sommet2 > maxDegre) {
            maxDegre = sommet2;
        }
    }
    graphe *g = malloc(sizeof(graphe));
    g->taille = maxDegre;

    fclose(fichier);

    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    g->tabOperations = malloc((maxDegre + 1) * sizeof(int));
    g->listeArc = malloc((maxNom + 1) * sizeof(sommetExclu));

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//lecture pour le degré des sommets à 0
        g->listeArc[sommet1].degre = 0;
        g->listeArc[sommet2].degre = 0;
    }
    fclose(fichier);
    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    for (int p = 0; p < g->taille+1; p++) {
        g->listeArc[p].degre = 0;
    }

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//lecture pour compter les degrés
        g->listeArc[sommet1].degre += 1;
        g->listeArc[sommet2].degre += 1;
    }

    for (int i = 1; i <= maxNom; i++) {
        g->listeArc[i].adjacents = malloc(g->listeArc[i].degre * sizeof(int));
        g->listeArc[i].nom = i;
    }

    fclose(fichier);
    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }
    int *index=calloc(g->taille+1,sizeof(int));
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//attribution des adjacences des sommets
        g->listeArc[sommet1].adjacents[index[sommet1]] = sommet2;
        g->listeArc[sommet2].adjacents[index[sommet2]] = sommet1;
        //printf("%d:%d\n",g->listeArc[sommet1].adjacents[index[sommet1]],g->listeArc[sommet2].adjacents[index[sommet2]]);
        index[sommet1] += 1;
        index[sommet2] += 1;
    }


    fclose(fichier);
    int degreMax = 0;
    for (int i = 1; i < g->taille+1; i++) {
        if (g->listeArc[i].degre > degreMax) {
            degreMax = g->listeArc[i].degre;
        }
    }
    g->degMax = degreMax;

    return g;
}

int *TriParDegreGraphe(graphe *g) {
    int finTri = g->degMax;
    int *tabSommets = calloc((g->taille) * sizeof(int), 0);
    for (int m = 0; m <= g->taille; m++) {
        tabSommets[m] = 0;
        g->listeArc[m].couleur=-1;
    }

    int j = 0;
    while (finTri > -1) {
        int i = 1;
        while (i <= 35) {
            if (g->listeArc[i].degre == finTri) {
                tabSommets[j] = i;
                j++;
            }
            i++;
        }
        finTri--;
    }

    return tabSommets;
}

bool estAdj(graphe *g, int s1, int s2) {
    for (int i = 0; i < g->listeArc[s1].degre; i++) {
        if (g->listeArc[s1].adjacents[i] == s2) {
            return true;
        }
    }
    return false;
}

bool estAdjTab(graphe *g, int sommet, int couleur) {
    for (int i = 0; i < g->listeArc[sommet].degre; i++) {
        for (int j = 0; j < g->taille; j++) {
            if (g->listeArc[g->listeArc[sommet].adjacents[i]].couleur == couleur) {
                if(estAdj(g,sommet,j)){
                    return true;
                }
            }
        }
    }
    return false;
}


void welshPowell(graphe *g ,operations_l **listeOP) {//Algorithme de Welsch et Powell comme étudié en cours
    printf("\nWELSCH POWELL\n");
    int *tabSommets = TriParDegreGraphe(g);
    printf(" ");
    int *vus=tabSommets;

    printf("debut algo\n");
    int couleur=0;
    for (int i = 0; i < g->taille; i++) {
        if(vus[i]!=0){
            g->listeArc[tabSommets[i]].couleur=couleur;
            printf("Couleur :%d\n%d,",i,tabSommets[i]);
            vus[i]=0;
            for(int k=0;k<g->taille;k++){
                if(vus[k]!=0&& !estAdj(g,tabSommets[i],vus[k])&& !estAdjTab(g,tabSommets[k],i)){
                    g->listeArc[tabSommets[k]].couleur=couleur;
                    for(int l=0;l<g->taille;l++){
                        if(tabSommets[k]==(*listeOP)[l].operation){
                            (*listeOP)[tabSommets[k]].couleur=couleur;}}
                    printf("%d,",tabSommets[k]);
                    vus[k]=0;

                }
            }
            couleur++;
        }




    }
    printf("fin algo\n");
    g->nbStations=couleur;
    printf("nbStations: %d\n",g->nbStations);
    free(tabSommets);//libération de la mémoire
    free(vus);
}

bool ErreurAllocation(int **colo,graphe *g){
    for(int i=0;i<g->taille;i++){
        if(colo[i][0]>(g->taille+1)){
            printf("erreur alloc\n");
            return false;
        }
    }
    printf("alloc faite\n");
    return true;
}



bool verifAdjacenceListeCouleurs(graphe* g, int** colorations) {
    for (size_t i = 0; i < g->taille; i++) {
        if (colorations[i][0] != 0 && colorations[i][0] <= g->taille) {
            for (int j = 0; j < g->taille; j++) {
                if (colorations[i][j] != 0 && colorations[i][j] <= g->taille) {
                    int sommet1 = colorations[i][j];
                    for (int k = 0; k < g->listeArc[sommet1].degre; k++) {
                        int adjacent = g->listeArc[sommet1].adjacents[k];
                        for (int l = 0; l < g->taille; l++) {
                            if (colorations[i][l] != 0 && colorations[i][l] <= g->taille) {
                                int sommet2 = colorations[i][l];
                                if (adjacent == sommet2) {

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}




#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H