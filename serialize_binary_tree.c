//program to demonstare binary tree serialization/deserialization
#include <stdio.h>
#define MARKER 0x41

struct Node
{
    int key;
    struct Node *left,*right;
    
};

//allocate node with value and null right/left leaves
Node* newNode(int key)
{
    Node* temp = new Node;
    temp->key = key;
    temp->left = temp->right = NULL;
    return (temp);
}

//serializa a tree in a file 
void serialize(Node *root, FILE *fp) 
{ 
    // If current node is NULL, store marker 
    if (root == NULL) 
    { 
        fprintf(fp, "%d ", MARKER); 
        return; 
    } 
  
    // Else, store current node and recur for its children 
    fprintf(fp, "%d ", root->key); 
    serialize(root->left, fp); 
    serialize(root->right, fp); 
} 
  
// This function constructs a tree from a file pointed by 'fp' 
void deSerialize(Node *&root, FILE *fp) 
{ 
    // Read next item from file. If theere are no more items or next 
    // item is marker, then return 
    int val; 
    if ( !fscanf(fp, "%d ", &val) || val == MARKER) 
       return; 
  
    // Else create node with this item and recur for children 
    root = newNode(val); 
    deSerialize(root->left, fp); 
    deSerialize(root->right, fp); 
} 

// A simple inorder traversal used for testing the constructed tree 
void inorder(Node *root) 
{ 
    if (root) 
    { 
        inorder(root->left); 
        printf("%d ", root->key); 
        inorder(root->right); 
    } 
} 

int main()
{
    struct Node *root = newNode(20);
    root->left               = newNode(8); 
    root->right              = newNode(22); 
    root->left->left         = newNode(4); 
    root->left->right        = newNode(12); 
    root->left->right->left  = newNode(10); 
    root->left->right->right = newNode(14); 
    FILE *fp = fopen("tree.txt","w");
    if(fp == NULL)
    {
        puts("Could not open file");
        return 0;
    }
    serialize(root,fp);
    fclose(fp);
    // Let us deserialize the storeed tree into root1 
    Node *root1 = NULL; 
    fp = fopen("tree.txt", "r"); 
    deSerialize(root1, fp); 
    
    printf("Inorder Traversal of the tree constructed from file:\n"); 
    inorder(root1); 
    
    return 0;
}