#ifndef BST_H
#define BST_H


typedef struct Node {
    void* data;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int isLeaf;
} Node;

typedef int (*compareFunc)(void* dataNew, void* dataExisting, void* context);

typedef struct BST {
    Node* root;
    compareFunc compare;
    size_t dataSize;
    unsigned int size;
    Node* smallestNode;
} BST;


BST* init_BST(size_t dataSize, compareFunc compare);

static Node* createNode(void* insertData, size_t dataSize);

Node* insert(void* insertData, BST* tree, void* context);

void delete(Node* deleteNode, BST* tree);

void printTree(Node* root, void (*printNode)(void* data));

void printBST(BST* tree, void (*printNode)(void* data));

Node* getSmallestLeaf(BST* tree);

void* getSmallestNodeData(BST* tree);

#endif