# GPS Problem Solver

## Description
Ce projet est une implémentation en langage C d’un **GPS (General Problem Solver)**, un système de résolution de problèmes logiques basé sur des règles.

Le programme tente d’atteindre un **état final** à partir d’un **état initial** en appliquant automatiquement des règles logiques.  
Il utilise un mécanisme de **chaînage avant** combiné à du **backtracking** pour explorer les différentes solutions possibles et revenir en arrière lorsqu’une impasse est atteinte.

Ce projet a été réalisé dans un cadre académique (Licence 2 Informatique).

---

## Fonctionnalités
- Lecture des règles et des états depuis un fichier texte
- Vérification automatique des préconditions des règles
- Application des règles (ajout et suppression de faits)
- Sauvegarde des états successifs
- Gestion du backtracking en cas d’impasse
- Détection automatique de l’atteinte du but

---

## Principe de fonctionnement
1. Lecture de l’état initial (`start`) et de l’état final (`finish`)
2. Chargement des règles (préconditions, ajouts, suppressions)
3. Recherche d’une règle applicable
4. Application de la règle et mise à jour de l’état
5. Sauvegarde de l’état courant
6. Retour en arrière (backtracking) si aucune règle applicable n’est trouvée
7. Arrêt lorsque le but est atteint ou qu’aucune solution n’est possible

---

## Technologies utilisées
- Langage C
- Structures de données (structures, tableaux)
- Manipulation de fichiers texte
- Algorithmes de recherche
- Raisonnement logique
- Backtracking

---
## Eexecution
```bash 
gcc gps.c -o gps
./gps

## Exemple de structure du fichier d’entrée
```txt
start: fait1,fait2
finish: fait_final

*
action: action1
preconds: fait1
add: fait3
delete: fait2



