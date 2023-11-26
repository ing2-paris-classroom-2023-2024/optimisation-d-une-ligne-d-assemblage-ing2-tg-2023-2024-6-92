#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Definition des structures

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

//Fonction qui lit le fichier
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
    int index = 0;
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        g->listeArc[sommet1].adjacents[index] = sommet2;
        g->listeArc[sommet2].adjacents[index] = sommet1;
        index += 1;
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

//Fonction qui trie dans l'ordre décroissant de degré
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

//Fonction qui détermine si un sommet est adjacent avec un autre
bool estAdj(graphe *g, int s1, int s2) {
    for (int i = 0; i < g->listeArc[s1].degre; i++) {
        if (g->listeArc[s1].adjacents[i] == s2) {
            return true;
        }
    }
    return false;
}

//Fonction coloration, qui va traiter les contraintes d'exclusion
Colorations *Coloration(graphe *g) {
    int couleur[g->taille];
    Colorations *colo = malloc(sizeof(Colorations));


    printf("ok\n");
    int *tabSommet = TriParDegreGraphe(g);

    int *vus = tabSommet;
    int fin = 0;
    printf("ok\n");

    printf("ok\n");
    while (fin < g->taille) {
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
                        printf("hihi\n");
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

//Fonction principale
int main() {
    graphe *g = lireFichier("exclusions.txt");
    Colorations *colo;
    colo = Coloration(g);
    for (int l = 0; l < g->taille; l++) {
        printf("\ncouleur %d :", l);
        for (int m = 0; m < g->taille; m++) {
            if (colo->Couleurs[l][m] != 0) {
                printf("%d,", colo->Couleurs[l][m]);
            }
        }
    }

    for (int i = 0; i < g->taille; i++) {
        free(colo->Couleurs[i]);
    }
    free(colo->Couleurs);
    free(colo);

    free(g->tabOperations);
    for (int i = 1; i <= g->taille; i++) {
        free(g->listeArc[i].adjacents);
    }
    free(g->listeArc);

    return 0;
}
