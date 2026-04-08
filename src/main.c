#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WORDS 3000
#define WORD_LEN 6

char words[MAX_WORDS][WORD_LEN];
int nb_words = 0;

void load_words(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur: impossible d'ouvrir le fichier\n");
        return;
    }
    char token[100];
    while (fscanf(f, "%s", token) == 1) {
        if (strlen(token) == 5) {
            int valide = 1;
            for (int i = 0; i < 5; i++) {
                if (!isalpha(token[i])) {
                    valide = 0;
                    break;
                }
                token[i] = tolower(token[i]);
            }
            if (valide) {
                strcpy(words[nb_words], token);
                nb_words++;
            }
        }
    }
    fclose(f);
}

int main() {
    load_words("ressource/bdd_wordle.txt");
    printf("Nombre de mots chargés : %d\n", nb_words);

    // Mot aléatoire
    srand(time(NULL));
    char *mot_secret = words[rand() % nb_words];

    char guess[100];
    int essais = 0;
    int gagne = 0;

    while (essais < 6 && !gagne) {
        printf("Proposer un mot : ");
        scanf("%s", guess);

        // Minuscules
        for (int i = 0; i < strlen(guess); i++)
            guess[i] = tolower(guess[i]);

        // Vérifier longueur
        if (strlen(guess) != 5) {
            printf("Le mot doit faire 5 lettres !\n");
            continue;
        }

        // Vérifier que le mot est dans la liste
        int valide = 0;
        for (int i = 0; i < nb_words; i++) {
            if (strcmp(words[i], guess) == 0) {
                valide = 1;
                break;
            }
        }
        if (!valide) {
            printf("Mot non reconnu, réessayez !\n");
            continue;
        }

        essais++;

        // Gagné ?
        if (strcmp(guess, mot_secret) == 0) {
            gagne = 1;
            break;
        }

        // Feedback
        char resultat[6];
        for (int i = 0; i < 5; i++) {
            if (guess[i] == mot_secret[i])
                resultat[i] = 'v';
            else if (strchr(mot_secret, guess[i]))
                resultat[i] = 'o';
            else
                resultat[i] = 'x';
        }
        resultat[5] = '\0';
        printf("%s\n", resultat);
    }

    // Fin de partie
    if (gagne)
        printf("Bravo, vous avez gagné en %d coups !\n", essais);
    else
        printf("Perdu ! Le mot était : %s\n", mot_secret);

    // Rejouer ?
    char reponse;
    printf("Voulez-vous rejouer ? (o/n) : ");
    scanf(" %c", &reponse);
    if (tolower(reponse) == 'o')
        main();

    return 0;
}
