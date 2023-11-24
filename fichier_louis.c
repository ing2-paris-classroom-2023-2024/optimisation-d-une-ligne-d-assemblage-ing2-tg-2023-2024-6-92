#include <stdio.h>
#include <stdlib.h>

typedef struct maillon{
    int note;
    struct maillon* suiv;
}t_maillon;


t_maillon* ajouterElement(t_maillon* ancre, int note_a_ajouter ){
    t_maillon *tmp = ancre;
    t_maillon *nouveau_maillon = (t_maillon *)(sizeof (t_maillon));
    nouveau_maillon->note= note_a_ajouter;
    if(ancre == NULL){
        return nouveau_maillon;
    }
    else{
        while(tmp->suiv !=NULL){
            tmp =tmp->suiv;
        }
    }
    return ancre;
}

t_maillon* depiler(t_maillon *ancre, t_maillon *maillon_depile){
    if(ancre == NULL){
        printf("la pile est déjà vide\n");
        return  NULL;
    }
    (*maillon_depile)  ;

}
typedef struct spectacle
{
    char * nom;
    float temps;
    struct spectacle * suivant;
} t_spectacle;

// Définition du type t_file
typedef struct file
{
    t_spectacle * premier;
    t_spectacle * dernier;
} t_file;

// 3.2 Prototype du sous-programme creerSpectacle
t_spectacle* creerSpectacle();

// 3.3 Implémentation du sous-programme creerSpectacle
t_spectacle* creerSpectacle() {
    t_spectacle * nouveau = (t_spectacle *) malloc(sizeof(t_spectacle));
    nouveau->nom = malloc(100 * sizeof(char));

    printf("Entrer le nom du spectacle : ");
    scanf("%s", nouveau->nom);

    printf("Entrer le temps du spectacle : ");
    scanf("%f", &(nouveau->temps));

    nouveau->suivant = NULL;

    return nouveau;
}

// 3.4 Implémentation du sous-programme ajoutSpectacle
void ajoutSpectacle( t_file * fileDyn, t_spectacle * nouveau){
    if(fileDyn->premier == NULL){
        fileDyn->premier = nouveau;
        fileDyn->dernier = nouveau;
    } else {
        fileDyn->dernier->suivant = nouveau;
        fileDyn->dernier = nouveau;
    }
}

// 3.5 Sous-programme afficherSpectacle
void afficherSpectacle(t_file * fileDyn) {
    t_spectacle * current;
    if(fileDyn->premier == NULL) {
        printf("La file est vide.\n");
        return;
    }
    while(fileDyn->premier != NULL) {
        current = fileDyn->premier;
        printf("Nom du spectacle : %s, Temps : %f\n", current->nom, current->temps);
        fileDyn->premier = fileDyn->premier->suivant;
        free(current->nom);
        free(current);
    }
}
int main() {
    printf("Hello, World!\n");
    return 0;
}