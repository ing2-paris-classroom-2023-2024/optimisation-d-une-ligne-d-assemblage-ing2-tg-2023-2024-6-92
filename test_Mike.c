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
        printf("%d,%d\n",sommet1,sommet2);
    }


    fclose(fichier);

    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }


    graphe *g=malloc(sizeof(graphe));
    g->tabOperations = malloc((maxDegre + 1) * sizeof(int));
    g->listeArc = malloc((maxNom + 1) * sizeof(sommet));




    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        // Mettre à jour le degré des sommets
        g->listeArc[sommet1].degre++;
        g->listeArc[sommet2].degre++;
        g->taille+=1;
    }


    for (int i = 0; i <= maxNom; i++) {
        g->listeArc[i].adjacents = malloc(g->listeArc[i].degre * sizeof(int));
        g->listeArc[i].degre = 0;
    }


    fclose(fichier);
    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        g->listeArc[sommet1].adjacents[g->listeArc[sommet1].degre++] = sommet2;
        g->listeArc[sommet2].adjacents[g->listeArc[sommet2].degre++] = sommet1;
    }

    fclose(fichier);
    return g;
}

int main() {
    graphe *g = lireFichier("exclusions.txt");


    for (int i = 0; i <= g->taille; i++) {
        printf("%d\n",g->listeArc[i].nom);
    }

    free(g->tabOperations);
    for (int i = 0; i < g->listeArc->nom; i++) {
        free(g->listeArc[i].adjacents);
    }
    free(g->listeArc);

    return 0;
}
