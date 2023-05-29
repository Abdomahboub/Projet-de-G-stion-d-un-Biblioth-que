#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Livre {
    int id;
    char titre[50];
    char auteur[50];
    char date[12];
};

struct Etudiant {
    int id;
    char nom[50];
    char classe[50];
    int numero;
    int idLivre;
    char date[12];
};

FILE* fichierLivre;
FILE* fichierEtudiant;
FILE* fichierTemp;
int trouve;

void viderBuffer();
void viderEcran();
void ajouterLivre();
void listerLivres();
void supprimerLivre();
void emprunterLivre();
void listerLivresEmpruntes();

int main() {
    int choix;

    while (1) {
        viderEcran();
        printf("<== Système de Gestion de Bibliothèque ==>\n");
        printf("1. Ajouter un livre\n");
        printf("2. Lister les livres\n");
        printf("3. Supprimer un livre\n");
        printf("4. Emprunter un livre\n");
        printf("5. Lister les livres empruntés\n");
        printf("0. Quitter\n\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 0:
                exit(0);

            case 1:
                ajouterLivre();
                break;

            case 2:
                listerLivres();
                break;

            case 3:
                supprimerLivre();
                break;

            case 4:
                emprunterLivre();
                break;

            case 5:
                listerLivresEmpruntes();
                break;

            default:
                printf("Choix invalide...\n\n");
                break;
        }

        printf("Appuyez sur Entrée pour continuer...");
        viderBuffer();
        getchar();
    }

    return 0;
}

void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void viderEcran() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ajouterLivre() {
    struct Livre livre;
    char dateActuelle[12];
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    sprintf(dateActuelle, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    strcpy(livre.date, dateActuelle);

    viderEcran();
    printf("<== Ajouter un livre ==>\n\n");

    fichierLivre = fopen("livres.dat", "ab");

    printf("Entrez l'identifiant du livre : ");
    scanf("%d", &livre.id);

    printf("Entrez le titre du livre : ");
    viderBuffer();
    fgets(livre.titre, 50, stdin);
    livre.titre[strcspn(livre.titre, "\n")] = '\0';

    printf("Entrez le nom de l'auteur : ");
    viderBuffer();
    fgets(livre.auteur, 50, stdin);
    livre.auteur[strcspn(livre.auteur, "\n")] = '\0';

    printf("Livre ajouté avec succès.\n");

    fwrite(&livre, sizeof(struct Livre), 1, fichierLivre);
    fclose(fichierLivre);
}

void listerLivres() {
    struct Livre livre;

    viderEcran();
    printf("<== Liste des livres disponibles ==>\n\n");
    printf("%-10s %-30s %-20s %s\n\n", "ID", "Titre", "Auteur", "Date");

    fichierLivre = fopen("livres.dat", "rb");

    while (fread(&livre, sizeof(struct Livre), 1, fichierLivre) == 1) {
        printf("%-10d %-30s %-20s %s\n", livre.id, livre.titre, livre.auteur, livre.date);
    }

    fclose(fichierLivre);
}

void supprimerLivre() {
    int idLivre;
    int trouve = 0;
    struct Livre livre;

    viderEcran();
    printf("<== Supprimer un livre ==>\n\n");
    printf("Entrez l'identifiant du livre à supprimer : ");
    scanf("%d", &idLivre);

    fichierLivre = fopen("livres.dat", "rb");
    fichierTemp = fopen("temp.dat", "wb");

    while (fread(&livre, sizeof(struct Livre), 1, fichierLivre) == 1) {
        if (livre.id == idLivre) {
            trouve = 1;
        } else {
            fwrite(&livre, sizeof(struct Livre), 1, fichierTemp);
        }
    }

    if (trouve == 1) {
        printf("\n\nSuppression réussie.");
    } else {
        printf("\n\nLivre non trouvé !");
    }

    fclose(fichierLivre);
    fclose(fichierTemp);

    remove("livres.dat");
    rename("temp.dat", "livres.dat");
}

void emprunterLivre() {
    struct Etudiant etudiant;
    struct Livre livre;
    char dateActuelle[12];
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    sprintf(dateActuelle, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    strcpy(etudiant.date, dateActuelle);

    trouve = 0;

    viderEcran();
    printf("<== Emprunter un livre ==>\n\n");
    printf("Entrez l'identifiant du livre à emprunter : ");
    scanf("%d", &etudiant.idLivre);

    // Vérifier si le livre existe
    fichierLivre = fopen("livres.dat", "rb");

    while (fread(&livre, sizeof(struct Livre), 1, fichierLivre) == 1) {
        if (livre.id == etudiant.idLivre) {
            strcpy(etudiant.nom, livre.titre);
            trouve = 1;
            break;
        }
    }

    if (trouve == 0) {
        printf("Aucun livre trouvé avec cet identifiant\n");
        printf("Veuillez réessayer...\n\n");
        return;
    }

    fichierEtudiant = fopen("etudiants.dat", "ab");

    printf("Entrez le nom de l'étudiant : ");
    viderBuffer();
    fgets(etudiant.nom, 50, stdin);
    etudiant.nom[strcspn(etudiant.nom, "\n")] = '\0';

    printf("Entrez la classe de l'étudiant : ");
    viderBuffer();
    fgets(etudiant.classe, 50, stdin);
    etudiant.classe[strcspn(etudiant.classe, "\n")] = '\0';

    printf("Entrez le numéro de l'étudiant : ");
    scanf("%d", &etudiant.numero);

    printf("Livre emprunté avec succès\n\n");

    fwrite(&etudiant, sizeof(struct Etudiant), 1, fichierEtudiant);
    fclose(fichierEtudiant);
}

void listerLivresEmpruntes() {
    struct Etudiant etudiant;

    viderEcran();
    printf("<== Liste des livres empruntés ==>\n\n");

    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "ID", "Nom", "Classe", "Numéro", "Titre du livre", "Date");

    fichierEtudiant = fopen("etudiants.dat", "rb");

    while (fread(&etudiant, sizeof(struct Etudiant), 1, fichierEtudiant) == 1) {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", etudiant.idLivre, etudiant.nom, etudiant.classe, etudiant.numero, etudiant.nom, etudiant.date);
    }

    fclose(fichierEtudiant);
}
