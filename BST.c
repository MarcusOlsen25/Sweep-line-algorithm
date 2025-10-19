#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "BST.h"

//Generic binary search tree with no balancing scheme. Data is stored in leaf nodes. Always contains 2n-1 nodes.

//Generic binary search tree containing pointer to root and metadata
//Takes as parameters a compare function for comparing nodes and size of data inserted
//Compare function takes a context parameter that allows for dynamic comparison in the sense that the value of a node
//may change and thereby recomputed based on the context passed to the function
BST* init_BST(size_t dataSize, compareFunc compare) {
    BST* bst = malloc(sizeof(BST));
    bst->root = NULL;
    bst->compare = compare;
    bst->dataSize = dataSize;
    bst->size = 0;
    bst->smallestNode = NULL;
    return bst;
}

//Allocate memory for new node of generic data
static Node* createNode(void* insertData, size_t dataSize) {
    Node* newNode = malloc(sizeof(Node));

    if (!newNode) return NULL;

    newNode->data = malloc(dataSize);
    if (!newNode->data) {
        free(newNode);
        return NULL;
    }
    if (insertData) {
        memcpy(newNode->data, insertData, dataSize);
    }

    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->isLeaf = true;
    return newNode;
}

void freeNode(Node* node) {
    free(node->data);
    free(node);
}

Node* getSibling(Node* node) {
    Node* parent = node->parent;
    Node* sibling;
    if (parent->left == node) {
        sibling = parent->right;
    } else {
        sibling = parent->left;
    }
    return sibling;
}

void* getSmallestNodeData(BST* tree) {
    if (tree->root) return tree->smallestNode->data;
    return NULL;
}

Node* getSmallestLeaf(BST* tree) {
    Node* root = tree->root;
    if (!root) return NULL;
    while (!root->isLeaf) {
        root = root->left;
    }
    return root;
}

//Insert new data. The placement is computed based on the compare function and the context. 
//The value of other nodes are recomputed based on the context
Node* insert(void* insertData, BST* tree, void* context) {
    // First insertion
    if (tree->size == 0) {
        Node* firstNode = createNode(insertData, tree->dataSize);
        tree->root = tree->smallestNode = firstNode;
        tree->size = 1;
        return firstNode;
    }

    // Find the leaf node to insert at
    Node* current = tree->root;
    while (!current->isLeaf) {
        if (tree->compare(insertData, current->data, context)) {
            current = current->right;
        } else {
            current = current->left;
        }
    }

    Node* parent = current->parent;

    Node* newInternal = createNode(NULL, tree->dataSize);
    newInternal->isLeaf = false;
    newInternal->parent = parent;

    Node* newLeaf = createNode(insertData, tree->dataSize);

    Node* leftLeaf, *rightLeaf;
    if (tree->compare(newLeaf->data, current->data, context)) {
        rightLeaf = newLeaf;
        leftLeaf = current;
    } else {
        leftLeaf = newLeaf;
        rightLeaf = current;
    }

    newInternal->left = leftLeaf;
    newInternal->right = rightLeaf;

    leftLeaf->parent = newInternal;
    rightLeaf->parent = newInternal;

    memcpy(newInternal->data, leftLeaf->data, tree->dataSize);

    if (parent) {
        if (parent->left == current) {
            parent->left = newInternal;
        } else {
            parent->right = newInternal;
        }
    } else {
        tree->root = newInternal;
    }

    tree->size += 2;

    //Set new smallest leaf
    if (!tree->compare(newLeaf->data, tree->smallestNode->data, context)) {
        tree->smallestNode = newLeaf;
    }

    return newLeaf;
}

//Delete deleteNode as well as its parent
void delete(Node* deleteNode, BST* tree) {
    if (!deleteNode) return;

    if (tree->size == 1) {
        freeNode(tree->root);
        tree->root = NULL;
        tree->size = 0;
        return;
    }
   
    Node* parent = deleteNode->parent;
    Node* grandParent = parent->parent;

    Node* sibling = getSibling(deleteNode);

    if (grandParent) {
        if (grandParent->left == parent) {
            grandParent->left = sibling;
        } else {
            grandParent->right = sibling;
        }

        sibling->parent = grandParent;
    } else {
        //sibling is now root
        sibling->parent = NULL;
        tree->root = sibling;
    }

    tree->size -= 2;

    if (tree->smallestNode == deleteNode) {
        tree->smallestNode = getSmallestLeaf(tree);
    }

    freeNode(deleteNode);
    freeNode(parent);
}

void _freeTree(Node* root) {
    if (!root) return;
    if (!root->isLeaf) {
        _freeTree(root->left);
        _freeTree(root->right);
    }
    free(root);
}

void freeTree(BST* tree) {
    if (!tree->root) return;
    _freeTree(tree->root);
}

void printTree(Node* root, void (*printNode)(void* data)) {
    if (!root) return;
    if (!root->isLeaf) {
        printTree(root->left, printNode);
        printTree(root->right, printNode);
    } else {
        printNode(root->data);
    }
}

void printBST(BST* tree, void (*printNode)(void* data)) {
    if (!tree->root) {
        printf("Empty\n\n");

        return;
    }
    printf("size: %d\n", tree->size);
    printTree(tree->root, printNode);
    printf("\n");
}