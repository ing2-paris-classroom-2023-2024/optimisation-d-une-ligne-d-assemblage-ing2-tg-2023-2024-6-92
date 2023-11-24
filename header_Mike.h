#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct sommet {
    int nom;
    int *adjacents;
    int degre;
} sommet;

typedef struct graphe {
    int *tabOperations;
    sommet *listeArc;
    int taille;
    int degMax;
} graphe;

typedef struct Colorations {
    int **Couleurs;
} Colorations;

graphe *lireFichier(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    int maxNom = 0;
    int maxDegre = 0;
    int sommet1, sommet2;

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
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

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
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

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
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
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        g->listeArc[sommet1].adjacents[index[sommet1]] = sommet2;
        g->listeArc[sommet2].adjacents[index[sommet2]] = sommet1;
        index[sommet1] += 1;
        index[sommet2] += 1;
    }
    printf("pb:%d\n",g->listeArc[15].adjacents[1]);

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
    for(int k=0;k<g->taille;k++){
        printf("\nsommet :%d => ",tabSommets[k]);
        for(int m=0;m<g->listeArc[tabSommets[k]].degre;m++){
            printf("%d,",g->listeArc[tabSommets[k]].adjacents[m]);
        }
    }
    printf("\n");
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

void welshPowell(graphe *g, Colorations *colorations) {
    int *tabSommets = TriParDegreGraphe(g);
    int *vus=tabSommets;
    for (int i = 0; i < g->taille; i++) {
        colorations->Couleurs[i] = malloc(g->taille*sizeof(int));
        if(vus[i]!=0){
            colorations->Couleurs[i][0] = tabSommets[i];
            printf("\nsommet %d: ",colorations->Couleurs[i][0]);
            vus[i]=0;
            int index=1;
            for(int k=0;k<g->taille;k++){
                if(vus[k]!=0&& !estAdj(g,tabSommets[i],vus[k])&& !estAdjTab(g,colorations->Couleurs[i],tabSommets[k],index)){
                    colorations->Couleurs[i][index]=tabSommets[k];
                    printf(" %d,",colorations->Couleurs[i][index]);
                    vus[k]=0;
                    index+=1;
                }

            }
        }





    }


    free(tabSommets);
    free(vus);
}




#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H
