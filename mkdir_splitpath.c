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
        return;  // If parent is NULL, splitPath has already printed the error.
    }

    // Check if the baseName is valid (it should not be empty)
    if (strlen(baseName) == 0) {
        printf("MKDIR ERROR: invalid directory name\n");
        return;
    }

    // Check if a directory with the same name already exists
    struct NODE* child = parent->childPtr;
    while (child) {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        child = child->siblingPtr;
    }

    // Allocate and initialize the new directory
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    if (!newDir) {
        printf("MKDIR ERROR: memory allocation failed\n");
        return;  // Check for memory allocation failure
    }
    
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->parentPtr = parent;
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;

    // Add newDir to parent's child list
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
    // Normalize pathName: remove multiple consecutive slashes
    char normalizedPath[128];
    char* src = pathName;
    char* dst = normalizedPath;

    while (*src) {
        // Skip consecutive slashes
        if (*src == '/' && (dst == normalizedPath || *(dst - 1) == '/')) {
            src++;
            continue;
        }
        *dst++ = *src++;
    }
    *dst = '\0';

    if (strcmp(normalizedPath, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    char* lastSlash = strrchr(normalizedPath, '/');
    if (lastSlash) {
        strncpy(dirName, normalizedPath, lastSlash - normalizedPath);
        dirName[lastSlash - normalizedPath] = '\0';
        strcpy(baseName, lastSlash + 1);
    } else {
        strcpy(dirName, "");
        strcpy(baseName, normalizedPath);
    }

    struct NODE* current = (dirName[0] == '/') ? root : cwd;

    // Tokenize the directory path and find the current directory
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


