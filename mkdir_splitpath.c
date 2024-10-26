#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

void mkdir(char pathName[]) {
    if (strlen(pathName) == 0 || strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64] = "";
    char dirName[64] = "";
    
    struct NODE* parent = splitPath(pathName, baseName, dirName);
    
    if (!parent) {
        return;
    }

    struct NODE* child = parent->childPtr;
    while (child) {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
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

struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    char* lastSlash = strrchr(pathName, '/');
    if (lastSlash) {
        strncpy(dirName, pathName, lastSlash - pathName);
        dirName[lastSlash - pathName] = '\0';
        strcpy(baseName, lastSlash + 1);
    } else {
        strcpy(dirName, "");
        strcpy(baseName, pathName);
    }

    // If dirName is empty, it means the directory is in the current directory.
    if (strcmp(dirName, "") == 0) {
        return cwd;
    }

    struct NODE* current = (dirName[0] == '/') ? root : cwd;

    char* token = strtok(dirName, "/");
    while (token != NULL) {
        struct NODE* child = current->childPtr;
        int found = 0;

        while (child) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                current = child;
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

    return current;
}

