#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

struct NODE *root;
struct NODE *cwd;
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "touch", "rm",
 "menu", "tree", "quit", NULL};


void ls(char* pathName) {
    printf("LS: List contents of %s\n", pathName);
}

void cd(char* pathName) {
    printf("CD: Change directory to %s\n", pathName);
}

void pwd() {
    printf("PWD: Current working directory is ");
}

void touch(char* pathName) {
    printf("TOUCH: Create file %s\n", pathName);
}

void rm(char* pathName) {
    printf("RM: Remove file %s\n", pathName);
}



int findCMD(char* command){
    int i = 0;
    while(cmd[i]){
        if(!strcmp(command, cmd[i])){
            return i;
        }
        i++;
    }
    return -1;
}


void initialize(){
    root = (struct NODE *)malloc(sizeof(struct NODE));
    root->parentPtr = NULL;
    root->siblingPtr = NULL;
    strcpy(root->name, "/");
    root->fileType = 'D';
    root->childPtr = NULL;
    cwd = root;
}


struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    strcpy(dirName, pathName);
    char* lastSlash = strrchr(dirName, '/');

    if (lastSlash) {
        strcpy(baseName, lastSlash + 1);
        if (lastSlash == dirName) {
            strcpy(dirName, "/");
        } else {
            *lastSlash = '\0';  
        }
    } else {
        strcpy(baseName, pathName);
        strcpy(dirName, "");
    }

    struct NODE* parent = root;
    if (strcmp(dirName, "/") != 0 && strlen(dirName) > 0) {
        char* token = strtok(dirName, "/");
        while (token) {
            struct NODE* child = parent->childPtr;
            int found = 0;
            while (child) {
                if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                    parent = child;
                    found = 1;
                    break;
                }
                child = child->siblingPtr;
            }
            if (!found) {
                printf("ERROR: directory %s does not exist\n", token);
                return NULL;
            }
            token = strtok(NULL, "/");
        }
    }
    return parent;
}


void mkdir(char* pathName) {
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char dirName[64], baseName[64];
    struct NODE* parent = splitPath(pathName, baseName, dirName);

    if (!parent) {
        return;
    }

    struct NODE* child = parent->childPtr;
    while (child) {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->parentPtr = parent;
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;

    if (parent->childPtr == NULL) {
        parent->childPtr = newDir;
    } else {
        struct NODE* lastChild = parent->childPtr;
        while (lastChild->siblingPtr) {
            lastChild = lastChild->siblingPtr;
        }
        lastChild->siblingPtr = newDir;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}

int main() {
    initialize();

    int index;
    char line[128];
    char command[16];
    char pathName[64];

    while (1) {
        strcpy(pathName, "/");
        fprintf(stderr, "input a command line: ");
        fgets(line, 128, stdin);
        line[strlen(line) - 1] = 0;
        sscanf(line, "%s %s", command, pathName);
        index = findCMD(command);

        switch (index) {
            case 0: mkdir(pathName); break;
            case 1: rmdir(pathName); break;
            case 2: ls(pathName); break;
            case 3: cd(pathName); break;
            case 4: pwd(); printf("\n"); break;
            case 5: touch(pathName); break;
            case 6: rm(pathName); break;
            case 7: menu(); break;
            case 8: printTree(); break;
            case 9: return quit();
            default: printf("invalid command %s\n", command);
        }
    }
}

void menu() {
    printf("Commands:\nmkdir [pathname], touch [pathname], rmdir [pathname], rm [pathname], ls [pathname], cd [pathname], pwd, menu, printTree, quit\n");
}

void printTree() {
    printf("%s", root->name);
    struct NODE *directory = root->childPtr;
    printf("\n");
    printHelper(directory, 0);
}

void printHelper(struct NODE *directory, int depth) {
    while (directory != NULL) {
        struct NODE *temp = directory;
        char spacing[128] = "";
        char t[128] = "";

        for (int i = 0; i < depth; i++) {
            if (temp->parentPtr->siblingPtr == NULL) {
                strcpy(t, "    ");
            } else {
                strcpy(t, "|   ");
            }

            strcat(t, spacing);
            strcpy(spacing, t);
            temp = temp->parentPtr;
        }

        printf("%s", spacing);
        if (directory->siblingPtr == NULL) {
            printf("`---");
        } else {
            printf("|---");
        }

        printf("%s\n", directory->name);
        if (directory->childPtr != NULL) {
            printHelper(directory->childPtr, depth + 1);
        }

        directory = directory->siblingPtr;
    }  
}

int quit() {
    return 0;
}
