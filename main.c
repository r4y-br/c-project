#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_POSTS 100
#define NAME_LENGTH 100
#define POST_LENGTH 255

typedef struct {
    int id;
    char name[NAME_LENGTH];
    int age;
} User;

typedef struct {
    int id;
    int userId;
    char content[POST_LENGTH];
} Post;

User users[MAX_USERS];
Post posts[MAX_POSTS];
int userCount = 0;
int postCount = 0;
int nextUserId = 1;
int nextPostId = 1;

void addUser() {
    if (userCount >= MAX_USERS) {
        printf("Erreur : Nombre maximum d'utilisateurs atteint.\n");
        return;
    }

    char name[NAME_LENGTH];
    int age;

    printf("Nom complet (prénom et nom) : ");
    getchar();
    fgets(name, NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // Remove trailing newline

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].name, name) == 0) {
            printf("Erreur : Le nom '%s' est déjà utilisé. Veuillez apporter une petite modification.\n", name);
            return;
        }
    }

    printf("Âge : ");
    scanf("%d", &age);

    users[userCount].id = nextUserId++;
    strcpy(users[userCount].name, name);
    users[userCount].age = age;
    userCount++;

    printf("Utilisateur ajouté : %s, ID : %d\n", name, nextUserId - 1);
}

void addPost() {
    if (postCount >= MAX_POSTS) {
        printf("Erreur : Nombre maximum de publications atteint.\n");
        return;
    }

    char name[NAME_LENGTH];
    char content[POST_LENGTH];
    int userId = -1;

    printf("Nom complet de l'utilisateur : ");
    getchar();
    fgets(name, NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // Remove trailing newline

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].name, name) == 0) {
            userId = users[i].id;
            break;
        }
    }

    if (userId == -1) {
        printf("Erreur : Utilisateur non trouvé.\n");
        return;
    }

    printf("Contenu de la publication : ");
    fgets(content, POST_LENGTH, stdin);
    content[strcspn(content, "\n")] = 0; // Remove trailing newline

    posts[postCount].id = nextPostId++;
    posts[postCount].userId = userId;
    strcpy(posts[postCount].content, content);
    postCount++;

    printf("Publication ajoutée avec succès.\n");
}

void displayUsers() {
    if (userCount == 0) {
        printf("Aucun utilisateur à afficher.\n");
        return;
    }

    printf("\nListe des utilisateurs :\n");
    for (int i = 0; i < userCount; i++) {
        printf("ID : %d, Nom : %s, Âge : %d\n", users[i].id, users[i].name, users[i].age);
    }
}

void displayUserPosts() {
    char name[NAME_LENGTH];
    int userId = -1;

    printf("Nom complet de l'utilisateur : ");
    getchar();
    fgets(name, NAME_LENGTH, stdin);
    name[strcspn(name, "\n")] = 0; // Remove trailing newline

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].name, name) == 0) {
            userId = users[i].id;
            break;
        }
    }

    if (userId == -1) {
        printf("Erreur : Utilisateur non trouvé.\n");
        return;
    }

    printf("\nPublications de %s :\n", name);
    for (int i = 0; i < postCount; i++) {
        if (posts[i].userId == userId) {
            printf("ID Publication : %d, Contenu : %s\n", posts[i].id, posts[i].content);
        }
    }
}

void saveData() {
    FILE *file = fopen("data.txt", "w");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier pour sauvegarde.\n");
        return;
    }

    fprintf(file, "%d\n", userCount);
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%d %s %d\n", users[i].id, users[i].name, users[i].age);
    }

    fprintf(file, "%d\n", postCount);
    for (int i = 0; i < postCount; i++) {
        fprintf(file, "%d %d %s\n", posts[i].id, posts[i].userId, posts[i].content);
    }

    fclose(file);
    printf("Données sauvegardées. Au revoir !\n");
}

void loadData() {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        printf("Aucun fichier de sauvegarde trouvé.\n");
        return;
    }

    fscanf(file, "%d\n", &userCount);
    for (int i = 0; i < userCount; i++) {
        fscanf(file, "%d %[^\n] %d\n", &users[i].id, users[i].name, &users[i].age);
    }

    fscanf(file, "%d\n", &postCount);
    for (int i = 0; i < postCount; i++) {
        fscanf(file, "%d %d %[^\n]\n", &posts[i].id, &posts[i].userId, posts[i].content);
    }

    nextUserId = userCount > 0 ? users[userCount - 1].id + 1 : 1;
    nextPostId = postCount > 0 ? posts[postCount - 1].id + 1 : 1;

    fclose(file);
}

int main() {
    int choice;
    loadData();

    do {
        printf("\n=== Mini Réseau Social ===\n");
        printf("1. Ajouter un utilisateur\n");
        printf("2. Ajouter une publication\n");
        printf("3. Afficher les utilisateurs\n");
        printf("4. Afficher les publications d'un utilisateur\n");
        printf("5. Sauvegarder et quitter\n");
        printf("Choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                addPost();
                break;
            case 3:
                displayUsers();
                break;
            case 4:
                displayUserPosts();
                break;
            case 5:
                saveData();
                break;
            default:
                printf("Option invalide. Veuillez réessayer.\n");
        }
    } while (choice != 5);

    return 0;
}
