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
} graphe;

graphe lireFichier(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int maxNom = 0;
    int maxDegre = 0;
    int sommet1, sommet2;

    // Trouver le nombre de sommets et le degré maximum
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

    // Réinitialiser le curseur de fichier
    fclose(fichier);

    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Initialiser le graphe
    graphe g;
    g.tabOperations = malloc((maxDegre + 1) * sizeof(int));
    g.listeArc = malloc((maxNom + 1) * sizeof(sommet));

    for (int i = 0; i <= maxNom; i++) {
        g.listeArc[i].nom = i;
        g.listeArc[i].degre = 0;
        g.listeArc[i].adjacents = NULL;
    }

    // Lire les arêtes et construire la liste d'adjacence
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        // Mettre à jour le degré des sommets
        g.listeArc[sommet1].degre++;
        g.listeArc[sommet2].degre++;
    }

    // Allouer de la mémoire pour les listes d'adjacence
    for (int i = 0; i <= maxNom; i++) {
        g.listeArc[i].adjacents = malloc(g.listeArc[i].degre * sizeof(int));
        g.listeArc[i].degre = 0;  // Réinitialiser le degré pour la construction de la liste d'adjacence
    }

    // Remplir les listes d'adjacence
    fclose(fichier);
    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        g.listeArc[sommet1].adjacents[g.listeArc[sommet1].degre++] = sommet2;
        g.listeArc[sommet2].adjacents[g.listeArc[sommet2].degre++] = sommet1;
    }

    fclose(fichier);
    return g;
}

int main() {
    graphe g = lireFichier("exclusions.txt");

    // Utilisez les données du graphe comme nécessaire

    // Libérez la mémoire à la fin du programme
    free(g.tabOperations);
    for (int i = 0; i < g.listeArc->nom; i++) {
        free(g.listeArc[i].adjacents);
    }
    free(g.listeArc);

    return 0;
}
