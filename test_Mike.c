#include <stdio.h>
#include <stdlib.h>

typedef struct sommet {
    int nom;
    int *adjacents;
    int degre;
} sommet;

typedef struct graphe {
    int *tabOperations;
    sommet *listeArc;
    int taille;
} graphe;

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
    graphe *g=malloc(sizeof(graphe));
    g->taille=maxDegre;


    fclose(fichier);

    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }



    g->tabOperations = malloc((maxDegre +1) * sizeof(int));
    g->listeArc = malloc((maxNom +1) * sizeof(sommet));


    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        g->listeArc[sommet1].degre=0;
        g->listeArc[sommet2].degre=0;
    }
    fclose(fichier);
    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {

        g->listeArc[sommet1].degre++;

        g->listeArc[sommet2].degre++;


    }
    printf("taille g:%d\n",g->taille);


    for (int i = 1; i <= maxNom; i++) {
        printf("%d\n",g->listeArc[i].degre);
        g->listeArc[i].adjacents = malloc(g->listeArc[i].degre * sizeof(int));
        g->listeArc[i].nom=i;

    }


    fclose(fichier);
    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }
    int index=0 ;
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        g->listeArc[sommet1].adjacents[index] = sommet2;

        g->listeArc[sommet2].adjacents[index] = sommet1;

        printf("%d,%d \n",g->listeArc[sommet1].adjacents[index],g->listeArc[sommet2].adjacents[index]);
        index+=1;

    }

    fclose(fichier);
    return g;
}

int main() {
    graphe *g = lireFichier("exclusions.txt");



    free(g->tabOperations);
    for (int i = 0; i < g->listeArc->nom; i++) {
        free(g->listeArc[i].adjacents);
    }
    free(g->listeArc);

    return 0;
}
