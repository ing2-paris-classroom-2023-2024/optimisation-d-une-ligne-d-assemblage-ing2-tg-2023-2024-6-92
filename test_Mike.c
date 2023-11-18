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



    for (int i = 1; i <= maxNom; i++) {

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


        index+=1;

    }

    fclose(fichier);
    return g;
}


int *TriParDegreGraphe(graphe *g){
    int degreMax=0 ;

    int *tabSommets = calloc((g->taille + 1) * sizeof(int),0);
    for (int m =0;m<=g->taille;m++){
        tabSommets[m]=0;
    }
    for (int i =1;i<g->taille;i++){
        if(g->listeArc[i].degre> degreMax){
            degreMax=g->listeArc[i].degre;
        }
    }

    int finTri=degreMax;
    int j=0;
    while(finTri>-1) {
        int i=1;
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

bool estAdj(graphe *g,int s1,int s2){
    for (int i=0;i<g->listeArc[s1].degre;i++){
        if(g->listeArc[s1].adjacents[i]==s2){
            return true;
        }
    }
    return false;
}



int main() {
    graphe *g = lireFichier("exclusions.txt");
    int *tabSommet=TriParDegreGraphe(g);
    for (int k =0;k<g->taille;k++){
        printf("%d,",tabSommet[k]);
    }

    free(g->tabOperations);
    for (int i = 0; i < g->listeArc->nom; i++) {
        free(g->listeArc[i].adjacents);
    }
    free(g->listeArc);

    return 0;
}
