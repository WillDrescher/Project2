#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

// Function to create a new directory
void mkdir(char pathName[]) {
    // Check if no path was provided
    if (strlen(pathName) == 0 || strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    // Prepare strings for baseName and dirName
    char baseName[64] = "";
    char dirName[64] = "";
    
    // Call splitPath to get the parent directory of the new directory
    struct NODE* parent = splitPath(pathName, baseName, dirName);
    
    // If the parent directory does not exist, splitPath will have already printed an error
    if (!parent) {
        return;
    }

    // Check if the directory already exists
    struct NODE* child = parent->childPtr;
    while (child) {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        child = child->siblingPtr;
    }

    // Create the new directory node
    struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newDir->name, baseName);
    newDir->fileType = 'D';
    newDir->parentPtr = parent;
    newDir->childPtr = NULL;
    newDir->siblingPtr = NULL;

    // Add newDir as the first child or as the last sibling of existing children
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

// Function to handle tokenizing and absolute/relative pathing
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    // Handle root path case
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    // Find the position of the last '/' in the pathName
    char* lastSlash = strrchr(pathName, '/');
    if (lastSlash) {
        // Split the pathName into dirName and baseName
        strncpy(dirName, pathName, lastSlash - pathName);
        dirName[lastSlash - pathName] = '\0';
        strcpy(baseName, lastSlash + 1);
    } else {
        // No '/' in pathName means it's just a name in the current directory
        strcpy(dirName, "");
        strcpy(baseName, pathName);
    }

    // Traverse the tree to find the parent directory
    struct NODE* current = (dirName[0] == '/') ? root : cwd;

    // Tokenize the dirName using '/' as the delimiter
    char* token = strtok(dirName, "/");
    while (token != NULL) {
        struct NODE* child = current->childPtr;
        int found = 0;

        // Search for the directory in the children
        while (child) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                current = child;
                found = 1;
                break;
            }
            child = child->siblingPtr;
        }

        // If a directory in the path is not found, print an error and return NULL
        if (!found) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }

        token = strtok(NULL, "/");
    }

    // Return the pointer to the directory where the new node should be created
    return current;
}
