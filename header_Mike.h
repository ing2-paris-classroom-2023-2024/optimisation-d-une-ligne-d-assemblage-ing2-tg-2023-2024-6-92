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

Colorations *Coloration(graphe *g) {
    int couleur[35];
    Colorations *colo = (Colorations*)malloc(sizeof(Colorations));


    printf("ok\n");
    int *tabSommet = TriParDegreGraphe(g);

    int *vus = tabSommet;
    int fin = 0;
    printf("ok\n");

    printf("ok\n");
    while (fin <= g->taille) {
        if (vus[fin] != 0) {
            printf("tour\n");

            for (int i = 0; i < g->taille; i++) {
                couleur[i] = 0;
            }
            couleur[0] = tabSommet[fin];

            vus[fin] = 0;
            int index = 1;
            for (int k = 0; k < g->taille; k++) {
                printf("elem\n");
                for (int j = 0; j < g->listeArc[tabSommet[k]].degre; j++) {
                    printf("boucle\n");
                    if ((!estAdj(g, tabSommet[k], g->listeArc[tabSommet[k]].adjacents[j]))&&(tabSommet[k] != g->listeArc[tabSommet[k]].adjacents[j])) {
                        printf("dansIF\n");
                        couleur[index] = tabSommet[k];

                        index++;
                    }
                }
            }
        }
        printf("fin\n");
        for(int k=0;k<g->taille;k++){
            colo->Couleurs[fin][k]=couleur[k];
        }
        printf("ici\n");
        fin++;
        for (int i = 0; i < g->taille; i++) {
            couleur[i] = 0;
        }
    }
    return colo;
}


void welshPowell(graphe *g, Colorations *colorations) {

    int *tabSommets = TriParDegreGraphe(g);
    printf("ok\n");
    for (int i = 0; i < g->taille; i++) {
        colorations->Couleurs[i][0] = 0;
    }
    printf("init\n");
    int couleurActuelle = 1;
    printf("pb:%d\n",g->listeArc[15].adjacents[0]);
    for (int i = 0; i < g->taille; i++) {

        int sommet = tabSommets[i];
        printf("sommet etudie:%d\n",sommet);
        for (int j = 0; j < g->listeArc[sommet].degre; j++) {
            printf("%d\n",g->listeArc[sommet].adjacents[j]);
                if (colorations->Couleurs[g->listeArc[sommet].adjacents[j]][0] == couleurActuelle) {
                    couleurActuelle++;
                    printf("meme couleur\n");
                    continue;
                }

            printf("c:%d\n",j);
        }

        colorations->Couleurs[sommet][0] = couleurActuelle;
        if (couleurActuelle > colorations->Couleurs[0][0]) {
            colorations->Couleurs[0][0] = couleurActuelle;
        }

        couleurActuelle = 1;
    }
    printf("fin\n");

    free(tabSommets);
}



#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_6_92_HEADER_MIKE_H
