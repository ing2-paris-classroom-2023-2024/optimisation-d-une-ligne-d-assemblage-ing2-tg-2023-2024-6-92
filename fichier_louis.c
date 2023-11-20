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

int main() {
    printf("Hello, World!\n");
    return 0;
}