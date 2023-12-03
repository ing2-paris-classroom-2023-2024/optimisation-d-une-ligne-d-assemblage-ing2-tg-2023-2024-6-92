#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define True 1
#define False 0

// STRUCTURE CONTENANT TOUTES LES INFOS SUR UNE OPERATION
typedef struct {
    int operation;                          // ID DE OPERATION
    float temps;                            // TEMPS NECESSAIRE POUR EFFECTUER OPERATION
    int* op_precedent;                      // LISTE DU NOMBRE OPERATION PRECEDENTE
    int nb_operation_precedente;            // INDIQUE LE NOMBRE OPERATION PRECEDENTE NECCASSAIRE POUR EFFECTUER OPERATION
    int effectuer;                          // INDIQUE SI OPERATION A ETE EFFECTUER, ELLE EST DEJA DANS UNE STATION
    int effectuable;                        // INDIQUE SI OPERATION EST EFFECTUABLE, TOUTES LES OPERATIONS PRECEDENTES SONT DEJA EFFECTUER
    int couleur;                            // INDIQUE LA COULEUR DE OPERATION GRACE A WESH POWELL
}operations_l;                              

typedef struct
{
    int op1;
    int op2;
}precedent;

typedef struct 
{
    int sommet;
    int nb;
}tuple;

// STRUCTURE STATION QUI CONTIENT LES INFOS
typedef struct 
{
    int nb_operation_max;                   // OPERATIONS MAXIMUM CONTENU DANS CETTE STATION
    int nb_operation_actuelle;              // OPERATION CONTENU A UN INSTANT T DANS CETTE STAION
    operations_l * liste_operation;         // LISTE OPERATION CONTENU DANS CETTE STATION
    float temps_total;                      // TEMPS DE PRIS PAR LA STATION (VS TEMPS DE CYCLE)
}station;                                    



typedef struct sommet {//structure d'un sommet avec la liste des sommets adjacents et son degré
    int nom;
    int *adjacents;
    int degre;
    int couleur;
} sommetExclu;

typedef struct graphe {//structure du graphe qui comprends la liste des sommets dans l'ordre décroissant des degrés, tous les sommets, la taille et le degré maximum
    int *tabOperations;
    sommetExclu *listeArc;
    int taille;
    int degMax;
    int nbStations;
} graphe;


int lecture_fichier_operation(operations_l** liste_operation);
float lecture_temps_cycle(void);
int nb_ligne_fichier(char* nom_fichier);
void precedence_init(int nombre_operation,operations_l** liste_operations);

int check_operation_possible(operations_l ** liste_operation,int nb_operation);
int comptage_operation(operations_l** liste_operation,int nb_operation);
void implementation_Pert(operations_l** liste_operation, int nombre_operation, int temps_cycle);
void associe_liste_operation_possible(operations_l** liste_operation,operations_l *** liste_operation_possible,int nb_operation);

graphe *lireFichier(const char *nomFichier);
int *TriParDegreGraphe(graphe *g);
bool estAdj(graphe *g,int s1,int s2);
bool estAdjTab(graphe *g, int sommet, int couleur);
void welshPowell(graphe *g ,operations_l **listeOP);



// Ouverture du fichier operation pour lecture des valeurs et inclusions dans une liste operation_l 
int lecture_fichier_operation(operations_l** liste_operation){

    FILE* fichier_ope;
    fichier_ope = fopen("operation.txt","r");
    if(fichier_ope==NULL){
        printf("Erreur lors de l'ouverture du fichier d'operation\n");
        return -1;
    }

    int operation_acutelle=0;
    float temps_actuelle=0;
    int operation_actuelle= 0;
    int nombre_operation = nb_ligne_fichier("operation.txt");  // permet de gérer le fait qu'on ne connaise pas le nombre d'opération à l'avance
    (*liste_operation) = malloc(sizeof(operations_l)*nombre_operation);

    while(fscanf(fichier_ope,"%i %f\n",&operation_acutelle,&temps_actuelle)==2){

        operation_actuelle++;
        (*liste_operation)[operation_actuelle-1].operation = operation_acutelle;
        (*liste_operation)[operation_actuelle-1].temps = temps_actuelle;
        (*liste_operation)[operation_actuelle-1].effectuer = False;
        (*liste_operation)[operation_actuelle-1].effectuable = False;
        operation_acutelle = 0;
        temps_actuelle = 0;
    }

    fclose(fichier_ope);
    return nombre_operation;
}

// Ouverture du fichier temps de cycle et lecture de celui-ci
float lecture_temps_cycle(void){
    FILE* fichier_temps;
    fichier_temps = fopen("temps_cycle.txt","r");
    if(fichier_temps==NULL){
        printf("Erreur lors de la lecture du fichier de temps de cycle");
        return -1;
    }

    float temps_cycle=0;
    fscanf(fichier_temps,"%f",&temps_cycle);
    fclose(fichier_temps);

    return temps_cycle;  
}

// Fonction comptant le nombre de ligne dans un fichier
int nb_ligne_fichier(char* nom_fcihier){
    FILE* fichier;
    fichier = fopen(nom_fcihier,"r");
    int nombre= 0;
    char c;

    while((c=fgetc(fichier))!=EOF){
        if(c=='\n')
            nombre++;
    }

    nombre++;
    fclose(fichier);
    return nombre;
}


// Ouvre et lis les informations contenu dans le fichier de precedence
void precedence_init(int nombre_operation,operations_l** liste_operations){

    // Initialisation du fichier
    int nb_arc= nb_ligne_fichier("precedences.txt");
    FILE *fichier_prece;
    fichier_prece = fopen("precedences.txt","r");
    if(fichier_prece==NULL){
        printf("Erreur lors de l'ouverture du fichier des precedences.");
        return;
    }
    // Precedent est un couple de sommets avec l'un precedant l'autre Ici on cree une liste de tous les couple present dans le fichier
    precedent * liste_precedent = (precedent*) malloc(sizeof(precedent)* nb_arc);
    int actuelle = 0;
    while(fscanf(fichier_prece,"%i %i\n",&liste_precedent[actuelle].op2,&liste_precedent[actuelle].op1)==2)
        actuelle++;
    // OP2 precede OP1 soit op2 doit etre execute avant op1

    // Une fois que tous les couples ont été lus on les reparti dans chaque operation correspondantes
    // On cree un tableau contenant les precedence
    int **liste_operation_precedente = (int**) malloc(sizeof(int*)*nombre_operation); 
    for(int i = 0;i<nombre_operation;i++){
        liste_operation_precedente[i] = (int*) malloc(sizeof(int)*nombre_operation);
    }
    int *nb_operation_precedentes = (int*) malloc(sizeof(int)*nombre_operation);
    for(int i = 0;i<nombre_operation;i++){
        nb_operation_precedentes[i] = 0;
    }
    int operation_precedente_actuelle = 0;
    for(int i = 0;i<nombre_operation;i++){
        operation_precedente_actuelle = 0;
        for(int j = 0;j<nombre_operation;j++){
            if(liste_precedent[j].op1 ==(*liste_operations)[i].operation){
                liste_operation_precedente[i][operation_precedente_actuelle] = liste_precedent[j].op2;
                operation_precedente_actuelle++;
                nb_operation_precedentes[i]++;
            }
        }
    }

    for(int i = 0;i<nombre_operation;i++){
        (*liste_operations)[i].nb_operation_precedente = nb_operation_precedentes[i];
        if(nb_operation_precedentes[i]==0)
            (*liste_operations)[i].effectuable = True;
        (*liste_operations)[i].op_precedent = malloc(sizeof(int)*nb_operation_precedentes[i]);
        for(int j = 0;j<nb_operation_precedentes[i];j++){
            (*liste_operations)[i].op_precedent[j] = liste_operation_precedente[i][j];
        }
    }

    fclose(fichier_prece);
    free(liste_precedent);
    for(int i = 0;i<nombre_operation;i++){
        free(liste_operation_precedente[i]);
    }
    free(liste_operation_precedente);
    free(nb_operation_precedentes);
}

/*
float calcul_chemin_rapide(operations_l*** operation_effectuable, int nb_operation_effectuable){
    //int nb_operation_effectuable = (int) (sizeof(*operation_effectuable)/sizeof(operations_l*));
    if(nb_operation_effectuable==1)
        return (*operation_effectuable)[0]->temps;
    float* chemins_possible = (float*) malloc(sizeof(int)*nb_operation_effectuable);
    float chemin_actuel;
    
    for(int operation_i = 0; operation_i<nb_operation_effectuable;operation_i++){
        int index = 0;
        operations_l** nouvelles_liste = (operations_l**) malloc(sizeof(operations_l*)*(nb_operation_effectuable-1));
        for(int operation_j = 0;operation_j<nb_operation_effectuable;operation_j++){
            if(operation_j!=operation_i){
                nouvelles_liste[index] = (*operation_effectuable)[operation_j];
                index++;
            }
        }
        chemin_actuel = calcul_chemin_rapide(&nouvelles_liste,nb_operation_effectuable-1);
        if(chemin_actuel+((*operation_effectuable)[operation_i])->temps<=10)
            chemins_possible[operation_i] = ((*operation_effectuable)[operation_i])->temps + chemin_actuel;
        else
            chemins_possible[operation_i] = chemin_actuel;
        free(nouvelles_liste);
    }
    float plus_rapide = 0;
    for(int chemins = 0; chemins < nb_operation_effectuable; chemins++){
        if(chemins_possible[chemins]>plus_rapide)
            plus_rapide = chemins_possible[chemins];
    }
    free(chemins_possible);
    return plus_rapide;
}
*/
// CETTE FONCTION EST OPERATIONELLE MAIS NECESSITE UNE GRANDE PUISSANCE DE CALCUL, CEPENDANT ELLE PEUT DONNER DES RESULTATS PLUS SATISFAISANT


int check_coloration(operations_l operation, station station_t){
    for(int operation_i = 0; operation_i<station_t.nb_operation_actuelle;operation_i++){
        if(operation.couleur==station_t.liste_operation[operation_i].couleur)
            return False;
    }
    return True;
}



float calcul_chemin_possible_rapide(operations_l*** operation_effectuable, int nb_operation_effectuable,float temps_actuel, station *station_t, int temps_cycle){
    float valeur_final = 0;
    float plus_grand = 0;
    int index_plus_grand =-1;
    if(nb_operation_effectuable ==0)
        return 0;
    for(int operation_i = 0; operation_i<nb_operation_effectuable;operation_i++){
        if((*operation_effectuable)[operation_i]->temps > plus_grand && (*operation_effectuable)[operation_i]->temps + temps_actuel <= temps_cycle && check_coloration(*((*operation_effectuable)[operation_i]),*station_t)){
            plus_grand = (*operation_effectuable)[operation_i]->temps;
            index_plus_grand = operation_i;
        }
    }
    if(index_plus_grand == -1)
        return 0;
    int index = 0;
    operations_l** nouvelles_liste = (operations_l**) malloc(sizeof(operations_l*)*(nb_operation_effectuable-1));
    for(int operation_j = 0;operation_j<nb_operation_effectuable;operation_j++){
        if(operation_j!=index_plus_grand){
            nouvelles_liste[index] = (*operation_effectuable)[operation_j];
            index++;
        }
    }
    (*operation_effectuable)[index_plus_grand]->effectuer = True;
    station_t->liste_operation[station_t->nb_operation_actuelle] = *((*operation_effectuable)[index_plus_grand]);
    station_t->nb_operation_actuelle++;
    valeur_final = (*operation_effectuable)[index_plus_grand]->temps + calcul_chemin_possible_rapide(&nouvelles_liste,index,temps_actuel+(*operation_effectuable)[index_plus_grand]->temps,station_t,temps_cycle);

    return valeur_final;
}




void implementation_Pert(operations_l** liste_operation, int nombre_operation,int temps_cycle){
    int nb_operation_restante = comptage_operation(liste_operation,nombre_operation);
    int nb_operation_possible = 0;
    int index_station = 0;
    station *liste_station = malloc(sizeof(station)*nombre_operation);
    while(nb_operation_restante!=0){
        nb_operation_possible = check_operation_possible(liste_operation,nombre_operation);
        operations_l **liste_operation_possible = (operations_l**) malloc(sizeof(operations_l*)*nb_operation_possible);
        associe_liste_operation_possible(liste_operation,&liste_operation_possible,nombre_operation);
        liste_station[index_station].nb_operation_max = nb_operation_possible;
        liste_station[index_station].nb_operation_actuelle = 0;
        liste_station[index_station].temps_total = 0;
        liste_station[index_station].liste_operation = malloc(sizeof(operations_l)*nb_operation_possible);
        liste_station[index_station].temps_total = calcul_chemin_possible_rapide(&liste_operation_possible,nb_operation_possible,0,&(liste_station[index_station]),temps_cycle);
        nb_operation_restante = comptage_operation(liste_operation,nombre_operation);
        free(liste_operation_possible);
        index_station++;
    }
    for(int i = 0;i<index_station;i++){
        printf("Les operations pour la station %i pour un temps total de %0.2f sont :",i+1,liste_station[i].temps_total);
        for(int j = 0; j<liste_station[i].nb_operation_actuelle;j++){
            printf(" %i ;",liste_station[i].liste_operation[j]);
        }
        printf("\n");
    }
}

void associe_liste_operation_possible(operations_l** liste_operation,operations_l *** liste_operation_possible,int nb_operation){
    int index = 0;
    for(int operation_i=0;operation_i<nb_operation;operation_i++){
        if((*liste_operation)[operation_i].effectuable == True && (*liste_operation)[operation_i].effectuer == False){
            (*liste_operation_possible)[index] = &((*liste_operation)[operation_i]);
            index++;
            
        }
    }
}




int check_operation_possible(operations_l ** liste_operation,int nb_operation){    // PARCOURS LA LISTE DES OPERATIONS AFIN DE METTRE A JOUR LEURS ETATS
    int actuelle_etat;
    int nb_operation_effectuable= 0;
    for(int operation_i = 0;operation_i<nb_operation;operation_i++){
        actuelle_etat = False;
        if((*liste_operation)[operation_i].effectuer==False){
            actuelle_etat = True;
            for(int op_precedente_i = 0;op_precedente_i<(*liste_operation)[operation_i].nb_operation_precedente;op_precedente_i++){
                for(int operation_j = 0;operation_j<nb_operation;operation_j++){
                    if((*liste_operation)[operation_i].op_precedent[op_precedente_i]==(*liste_operation)[operation_j].operation){
                        if((*liste_operation)[operation_j].effectuer==False)
                            actuelle_etat = False;
                    }
                }
            }
            if(actuelle_etat==True){
                (*liste_operation)[operation_i].effectuable = True;
                nb_operation_effectuable++;
            }

        }
    }
    return nb_operation_effectuable;
}

int comptage_operation(operations_l** liste_operation,int nb_operation){    // COMPTE LE NOMBRE OPERATION RESTANTE, LES OPERATIONS QUI N ONT PAS ENCORE ETE EXECUTE
    int nb_operation_restante= 0;
    for(int operation_i = 0;operation_i<nb_operation;operation_i++){        // POUR CHAQUE OPERATION, ON REGARDE SI ELLE A DEJA ETE EXECUTE OU NON
        if((*liste_operation)[operation_i].effectuer==False)
            nb_operation_restante++;
    }

    return nb_operation_restante;
}





graphe *lireFichier(const char *nomFichier) {//Fonction qui permet de lire le fichier de contrainte d'exclusions
    //suite à des problèmes d'allocations dynamique nous avons décidé de lire plusieurs fois le fichier
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    int maxNom = 0;
    int maxDegre = 0;
    int sommet1, sommet2;

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//première lecture pour avoir le degré max
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
    g->listeArc = malloc((maxNom + 1) * sizeof(sommetExclu));

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//lecture pour le degré des sommets à 0
        g->listeArc[sommet1].degre = 0;
        g->listeArc[sommet2].degre = 0;
    }
    fclose(fichier);
    fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("probleme fichier");
        exit(EXIT_FAILURE);
    }

    for (int p = 0; p < g->taille+1; p++) {
        g->listeArc[p].degre = 0;
    }

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//lecture pour compter les degrés
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
    int *index=calloc(g->taille+1,sizeof(int));
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {//attribution des adjacences des sommets
        g->listeArc[sommet1].adjacents[index[sommet1]] = sommet2;
        g->listeArc[sommet2].adjacents[index[sommet2]] = sommet1;
        //printf("%d:%d\n",g->listeArc[sommet1].adjacents[index[sommet1]],g->listeArc[sommet2].adjacents[index[sommet2]]);
        index[sommet1] += 1;
        index[sommet2] += 1;
    }


    fclose(fichier);
    int degreMax = 0;
    for (int i = 1; i < g->taille+1; i++) {
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
        g->listeArc[m].couleur=-1;
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

bool estAdjTab(graphe *g, int sommet, int couleur) {
    for (int i = 0; i < g->listeArc[sommet].degre; i++) {
        for (int j = 0; j < g->taille; j++) {
            if (g->listeArc[g->listeArc[sommet].adjacents[i]].couleur == couleur) {
                if(estAdj(g,sommet,j)){
                    return true;
                }
            }
        }
    }
    return false;
}


void welshPowell(graphe *g ,operations_l **listeOP) {//Algorithme de Welsch et Powell comme étudié en cours
    int *tabSommets = TriParDegreGraphe(g);
    int *vus=tabSommets;

    int couleur=0;
    for (int i = 0; i < g->taille; i++) {
        if(vus[i]!=0){
            g->listeArc[tabSommets[i]].couleur=couleur;
            vus[i]=0;
            for(int k=0;k<g->taille;k++){
                if(vus[k]!=0&& !estAdj(g,tabSommets[i],vus[k])&& !estAdjTab(g,tabSommets[k],i)){
                    g->listeArc[tabSommets[k]].couleur=couleur;
                    for(int l=0;l<g->taille;l++){
                        if(tabSommets[k]==(*listeOP)[l].operation){
                            (*listeOP)[tabSommets[k]].couleur=couleur;}}
                    vus[k]=0;

                }
            }
            couleur++;
        }
    }

    g->nbStations=couleur;
    free(tabSommets);//libération de la mémoire
    free(vus);
}

bool ErreurAllocation(int **colo,graphe *g){
    for(int i=0;i<g->taille;i++){
        if(colo[i][0]>(g->taille+1)){
            printf("erreur alloc\n");
            return false;
        }
    }
    return true;
}



bool verifAdjacenceListeCouleurs(graphe* g, int** colorations) {
    for (size_t i = 0; i < g->taille; i++) {
        if (colorations[i][0] != 0 && colorations[i][0] <= g->taille) {
            for (int j = 0; j < g->taille; j++) {
                if (colorations[i][j] != 0 && colorations[i][j] <= g->taille) {
                    int sommet1 = colorations[i][j];
                    for (int k = 0; k < g->listeArc[sommet1].degre; k++) {
                        int adjacent = g->listeArc[sommet1].adjacents[k];
                        for (int l = 0; l < g->taille; l++) {
                            if (colorations[i][l] != 0 && colorations[i][l] <= g->taille) {
                                int sommet2 = colorations[i][l];
                                if (adjacent == sommet2) {

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

