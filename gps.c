#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


typedef char string[1024];

// Structures de données
struct regle {
    string action;  // action que fait la règle
    string Preconds[10]; // précondition d'une règle
    int nb_precon;
    string addition[10]; // faits à ajouter à l'état
    int nb_add;
    string delete[10]; // faits à enlever à l'état
    int nb_del;
};

struct etat {
    string faits[20];       // Stocke les faits courants
    int nb_faits;           // Nombre de faits actuels
    int regle_appliquee;    // Index de la règle utilisée pour arriver ici
};

// Variables globales
int nb_regle = -1;
string okay;
struct regle regles[20];
struct etat historique[100];   // Stocke tous les états visités
int current_etat = 0;          // un index qui pointe vers le dernier état sauvegardé
int IR = 0;                     // Prochaine règle à tester
bool regles_appliquees[20] = {false};   // Suivi des règles déjà utilisées

// Fonctions utilitaires

// fonction donnée dans la partie 1 permettant de récupèrer chaque mot d'une ligne de texte se trouvant après les ':'
int parseLine(char source[], string cible[]) {
    int i = 0, n = 0;
    while (source[i] != ':') i++;
    i++;
    int j = i;
    while (source[i] != '\n' && source[i] != '\0') {
        if (source[i] == ',') {
            memcpy(&cible[n], &source[j], i - j);
            cible[n][i - j] = '\0';
            n++;
            j = i + 1;
        }
        i++;
    }
    if (j < i) {
        memcpy(&cible[n], &source[j], i - j);
        cible[n][i - j] = '\0';
        n++;
    }
    return n;
}

// permet de récupèrer le premier mot d'une ligne d'un fichier texte
void word(char source[]) {
    int i = 0;
    string mot;
    while (source[i] != ':') i++;
    memcpy(mot, "          ", 11);
    memcpy(mot, &source[0], i);
    strcpy(okay, mot);
}

//permet de savoir si une règle est applicable selon l'état ou non
int applicable(string situation[], struct regle reg, int sit) {
    int nb_egal = 0;
    for (int i = 0; i < sit; i++) {
        for (int j = 0; j < reg.nb_precon; j++) {
            if (strcmp(situation[i], reg.Preconds[j]) == 0) {
                nb_egal++;
            }
        }
    }
    return nb_egal == reg.nb_precon;
}

/* fonctions pour le backtracking,Le backtracking est déclenché quand :
Aucune règle ne peut être appliquée.
Le but n'est pas atteint après plusieurs essais.
Le finish dans le fichier définit les faits du but (la condition de victoire).
Le programme compare en permanence l'état actuel (situation[]) avec ces faits pour savoir s’il a réussi.






Cette fonction vérifie si l'état actuel (situation) contient tous les faits du but (but).
C'est le "test de victoire" qui détermine quand arrêter la recherche.
C'est elle qui compare l'état actuel avec le but final.*/
bool but_atteint(string situation[], string but[], int nb_sit, int nb_but) {
    int nb_egal = 0;
    for (int i = 0; i < nb_but; i++) {// Pour chaque fait du but
        for (int j = 0; j < nb_sit; j++) {// On cherche dans les faits actuels
            if (strcmp(situation[j], but[i]) == 0) {// Si on trouve une correspondance
                nb_egal++;
                break;
            }
        }
    }
    return nb_egal == nb_but;
}




/*Enregistre l'état actuel dans l'historique avant d'appliquer une nouvelle règle
Copie tous les faits (situation) dans l'historique.
Stocke le numéro de la règle utilisée (regle_appliquee).
Incrémente le compteur d'états (current_etat).
Permet de revenir en arrière si une séquence mène à une impasse.*/
void sauvegarder_etat(string situation[], int nb_sit, int regle_appliquee) {
    if (current_etat >= 100) return;
    // Copie des faits
    for (int i = 0; i < nb_sit; i++) {
        strcpy(historique[current_etat].faits[i], situation[i]);
    }
    historique[current_etat].nb_faits = nb_sit;
    historique[current_etat].regle_appliquee = regle_appliquee;
    current_etat++;
}

//Annule la dernière décision quand on atteint une impasse
void revenir_etat_precedent(string situation[], int nb_sit_arr[1]) {
    if (current_etat <= 1) return;// Impossible de revenir plus loin
    current_etat--; // Recule dans l'historique
    int prev_etat = current_etat - 1;
    // Restaure les faits
    for (int i = 0; i < historique[prev_etat].nb_faits; i++) {
        strcpy(situation[i], historique[prev_etat].faits[i]);
    }
    nb_sit_arr[0] = historique[prev_etat].nb_faits;
    // Réactive la règle qui a mené à l'impasse
    int regle_abandonnee = historique[current_etat].regle_appliquee;
    if (regle_abandonnee >= 0 && regle_abandonnee <= nb_regle) {
        regles_appliquees[regle_abandonnee] = false;
    }
}


// permet d'appliquer une règle (donc enlever et ajouter les faits étant avec la règle appliquée)
void appliquer_regle(int regle_index, string situation[], int nb_sit_arr[1]) {
    int nb_sit = nb_sit_arr[0];

    printf("\n___________________________________\n");
    printf("Regle numero %d est appliquee : %s\n", regle_index + 1, regles[regle_index].action);

    // Suppression des faits
    for (int i = 0; i < regles[regle_index].nb_del; i++) {
        for (int j = 0; j < nb_sit; j++) {
            if (strcmp(situation[j], regles[regle_index].delete[i]) == 0) {
                printf("A retire : %s\n", situation[j]);
                for (int h = j; h < nb_sit - 1; h++) {
                    strcpy(situation[h], situation[h + 1]);
                }
                nb_sit--;
                break;
            }
        }
    }

    // Ajout des nouveaux faits
    printf("elements a ajouter : ");
    for (int i = 0; i < regles[regle_index].nb_add; i++) {
        printf("%s ", regles[regle_index].addition[i]);
    }
    printf("\n");

    for (int i = 0; i < regles[regle_index].nb_add; i++) {
        bool deja_present = false;
        for (int j = 0; j < nb_sit; j++) {
            if (strcmp(situation[j], regles[regle_index].addition[i]) == 0) {
                deja_present = true;
                break;
            }
        }
        if (!deja_present) {
            strcpy(situation[nb_sit], regles[regle_index].addition[i]);
            nb_sit++;
        }
    }

    printf("Situation apres modification : ");
    for (int g = 0; g < nb_sit; g++) {
        printf("%s ", situation[g]);
    }
    printf("\n");

    nb_sit_arr[0] = nb_sit;
    regles_appliquees[regle_index] = true;
}

// Fonction principale
int main() {
    FILE *monflux = fopen("loup.txt", "r"); // on ouvre le fichier texte dns lequel on veut récupèrer les informations

    char c[80];
    string start[15], end[5];
    int nb_sit = 0, nb_end = 0;
    int nb_sit_arr[1] = {0};

    // Lecture du fichier
    while (fgets(c, 80, monflux) != NULL) {
        word(c);

        if (strncmp(okay, "start", 3) == 0) {
            nb_sit = parseLine(c, start);
        } 
        else if (strncmp(okay, "finish", 3) == 0) {
            nb_end = parseLine(c, end);
        } 
        else if (c[0] == '*') {
            nb_regle++;
        } 
        else if (strncmp(okay, "action", 3) == 0) {
            parseLine(c, &regles[nb_regle].action);
        } 
        else if (strncmp(okay, "preconds", 3) == 0) {
            regles[nb_regle].nb_precon = parseLine(c, regles[nb_regle].Preconds);
        } 
        else if (strncmp(okay, "add", 3) == 0) {
            regles[nb_regle].nb_add = parseLine(c, regles[nb_regle].addition);
        } 
        else if (strncmp(okay, "delete", 3) == 0) {
            regles[nb_regle].nb_del = parseLine(c, regles[nb_regle].delete);
        }
    }
    fclose(monflux); // on ferme le fichier une fois les informations récupèrer

    nb_sit_arr[0] = nb_sit;
    sauvegarder_etat(start, nb_sit, -1);
    bool fini = but_atteint(start, end, nb_sit, nb_end);
    bool possible = true;

    // Algorithme principal avec backtracking
    while (!fini && possible) { //Le but n'est pas atteint Et qu'une solution est encore possible (possible).
        // 1. Cherche une règle applicable non utilisée
        bool regle_trouvee = false;
        int regle_index = -1;

        for (int i = IR; i <= nb_regle && !regle_trouvee; i++) { // Commence à IR (évite de re-tester les règles déjà examinées).
            if (!regles_appliquees[i] && applicable(start, regles[i], nb_sit_arr[0])) {
                regle_index = i;
                regle_trouvee = true;
            }
        }
        // 2. Si trouvée, l'appliquer
        if (regle_trouvee) {
            appliquer_regle(regle_index, start, nb_sit_arr); // Modifie l'état actuel (ajout/suppression de faits)
            nb_sit = nb_sit_arr[0]; //Met à jour le nombre de faits après modification.
            sauvegarder_etat(start, nb_sit, regle_index);
            IR = 0;// Réinitialise l'index de recherche pour la prochaine itération
        }
         // 3. Sinon, backtrack
        else {
            if (current_etat > 1) {
                IR = historique[current_etat-1].regle_appliquee + 1;
                // Essaye la règle suivante
                revenir_etat_precedent(start, nb_sit_arr);
                nb_sit = nb_sit_arr[0];
                printf("Backtrack: retour a l'etat precedent\n");
            } else {
                possible = false;// Plus de solutions
            }
        }

        fini = but_atteint(start, end, nb_sit, nb_end);
    }

    if (fini) {
        printf("\nBravo ! Le programme a réussi à trouver une solution ! \n");
    } else {
        printf("\nAucune solution trouvee.\n");
    }

    return 0;
}
