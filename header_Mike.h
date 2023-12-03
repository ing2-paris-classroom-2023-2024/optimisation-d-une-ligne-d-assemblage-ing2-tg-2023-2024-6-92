#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct sommet {//structure d'un sommet avec la liste des sommets adjacents et son degre
    int nom;
    int *adjacents;
    int degre;
} sommet;

typedef struct graphe {//structure du graphe qui comprends la liste des sommets dans l'ordre decroissant des degres, tous les sommets, la taille et le degre maximum
    int *tabOperations;
    sommet *listeArc;
    int taille;
    int degMax;
} graphe;

typedef struct Colorations {//structure qui contient un tableau des couleurs dans lesquelles on a les sommets
    int **Couleurs;
} Colorations;

graphe *lireFichier(const char *nomFichier) {//Fonction qui permet de lire le fichier de contrainte d'exclusions
    //suite à des problemes d'allocations dynamique nous avons decide de lire plusieurs fois le fichier
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    int maxNom = 0;
    int maxDegre = 0;
    int sommet1, sommet2;

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//premiere lecture pour avoir le degre max
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
    g->listeArc = malloc((maxNom + 1) * sizeof(sommet));

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//lecture pour le degre des sommets à 0
        g->listeArc[sommet1].degre = 0;
        g->listeArc[sommet2].degre = 0;
    }
    fclose(fichier);
    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    for (int p = 0; p < g->taille; p++) {
        g->listeArc[p].degre = 0;
    }

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//lecture pour compter les degres
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
    int *index=calloc(g->taille,sizeof(int));
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//attribution des adjacences des sommets
        g->listeArc[sommet1].adjacents[index[sommet1]] = sommet2;
        g->listeArc[sommet2].adjacents[index[sommet2]] = sommet1;
        index[sommet1] += 1;
        index[sommet2] += 1;
    }


    fclose(fichier);
    int degreMax = 0;
    for (int i = 1; i < g->taille; i++) {
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

bool estAdjTab(graphe *g, int *tab, int sommet, size_t index) {
    for (int i = 0; i < g->listeArc[sommet].degre; i++) {
        for (size_t j = 0; j < index; j++) {
            if (g->listeArc[sommet].adjacents[i] == tab[j]) {
                return true;
            }
        }
    }
    return false;
}

void welshPowell(graphe *g, Colorations *colorations) {//Algorithme de Welsch et Powell comme etudie en cours
    int *tabSommets = TriParDegreGraphe(g);
    int *vus=tabSommets;
    for (int i = 0; i < g->taille; i++) {
        colorations->Couleurs[i] = malloc(g->taille*sizeof(int));
        if(vus[i]!=0){
            colorations->Couleurs[i][0] = tabSommets[i];
            printf("\n");
            vus[i]=0;
            int index=1;
            for(int k=0;k<g->taille;k++){
                if(vus[k]!=0&& !estAdj(g,tabSommets[i],vus[k])&& !estAdjTab(g,colorations->Couleurs[i],tabSommets[k],index)){
                    colorations->Couleurs[i][index]=tabSommets[k];
                    printf(" ");
                    vus[k]=0;
                    index+=1;
                }

            }
        }
    }
    free(tabSommets);//liberation de la memoire
    free(vus);
}




#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H
