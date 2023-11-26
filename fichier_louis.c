void pert_algo(int nombre_operations, operations_l **liste_operations,float temps_cycle){ // ici on va essayer d'implementer la methode de Pert qui ressemble beaucoup à notre cas d'usage
    int** liste_operations_effectuer = (int*) malloc(sizeof(int)*nombre_operations);     //liste des operations qui ont deja ete effectue par cycle, quand i = 0 alors c'est le premier cycle toutes les operations effectue
    for(int i = 0;i<nombre_operations;i++){
        liste_operations_effectuer[i] = (int*) malloc(sizeof(int)*nombre_operations);
    }
    int * operations_effectuer = (int*) malloc(sizeof(int)*nombre_operations);           // cette liste nous permet d'acceder plus rapidement et plus facilement a toutes les operations effectuer sans avoir a faire un balayge en 2D
    int* operations_facile = (int*) malloc(sizeof(int) * nombre_operations);            //liste des operations consideré comme facile à effectue car elle ne requiert aucunes autre tache pour etre effectue
    int operations_trie_precedence = (int) malloc(sizeof(int)*nombre_operations);     // liste qui contient les operations trie par ordre croissant de nombre de precedances, plus une operation à besoin de precedences plus elle sera mise vers le fin de la liste
    int* operations_possible = (int*) malloc(sizeof(int)*nombre_operations);            //liste des operations pouvants etre effectue à un instant T
    int operations_facile_index = -1;
    for(int operation_actuelle=0;operation_actuelle<nombre_operations;operation_actuelle++){
        if((*liste_operations)[operation_actuelle].nb_operation_precedente==0)
            operations_facile[operations_facile_index++] = (*liste_operations)[operation_actuelle].operation;

    }



}

void tri_ordre_operation(tuple** operations_trie, operations_l** liste_operations,int nombre_operations){
    tuple provisoire;
    for(int i = 0;i<nombre_operations;i++){
        (*operations_trie)[i].sommet = (*liste_operations)[i].operation;
        (*operations_trie)[i].nb = (*liste_operations)[i].nb_operation_precedente;
    }
    for(int i = 0; i<nombre_operations;i++){
        for(int j = 0;j<nombre_operations;j++){
            if((*operations_trie)[j].nb>(*operations_trie)[j+1].nb){
                provisoire.nb = (*operations_trie)[j].nb;
                provisoire.sommet = (*operations_trie)[j].sommet;
                (*operations_trie)[j].nb = (*operations_trie)[j+1].nb;
                (*operations_trie)[j].sommet = (*operations_trie)[j+1].sommet;
                (*operations_trie)[j+1].nb = provisoire.nb;
                (*operations_trie)[j+1].sommet = provisoire.sommet;

            }
        }
    }
}



void update_operations_possible(int index_operation_facile,int **operation_possible, int nombre_operation, int * operation_effectuer, int * opeartions_facile, operations_l **liste_operation,float temps_cycle){  //cette fonction va nous permettre de mettre a jour notre liste contenant les operations effectuables à un instant T
    int nombre_operation_possible = 0;
    int possible = 1;
    for(int i = 0;i<index_operation_facile;i++){
        (*operation_possible)[i] = opeartions_facile[i];
        nombre_operation_possible++;
    }
    if(calcul_operations_temps(*operation_possible,liste_operation,nombre_operation,nombre_operation_possible,temps_cycle)==1)
        return;
    for(int operation_i = 0;operation_i<nombre_operation;operation_i++){
        for(int operation_j=0;operation_j<(*liste_operation)[operation_i].nb_operation_precedente;operation_j++){
            possible = 1;
            for(int operation_effectue = 0; operation_effectue < sizeof(operation_effectuer)/sizeof(int);operation_effectue++){            // ca ne fonctionne pas mais c'est un debut 
                if((*liste_operation)[operation_i].operation_precedente[operation_j] != operation_effectuer[operation_effectue]){
                    possible=1;
                }
            }
            if(possible==1){
                (*operation_possible)[nombre_operation_possible] = (*liste_operation)[operation_i].operation;
                nombre_operation_possible++;
            }
        }
    }
    return;
}

int calcul_operations_temps(int* liste,operations_l** liste_operations,int nb_operations, int nombre_operations_possible,float temps_cyle){
    float time = 0;
    for(int i = 0;i<nombre_operations_possible;i++){
        for(int j = 0;j<nb_operations;j++){
            if(j = liste[i])
                time+= (float) (*liste_operations)[j].temps;
        }
    }
    if(time> temps_cyle)
        return 1;
    return 0;
}
