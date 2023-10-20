#include <stdio.h>
#include <stdlib.h>

// Estrutura de um nó da árvore
typedef struct Node {
    int data;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    int color;  // 0 for Black, 1 for Red
} Node;

// Função para criar um novo nó
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->color = 1;  // Novos nós são sempre vermelhos
    return newNode;
}

// Função para realizar uma rotação à esquerda
void leftRotate(Node** root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Função para realizar uma rotação à direita
void rightRotate(Node** root, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// Função para balancear a árvore após a inserção de um nó
void fixInsertion(Node** root, Node* newNode) {
    Node* parent = NULL;
    Node* grandparent = NULL;

    while ((newNode != *root) && (newNode->color == 1) && (newNode->parent->color == 1)) {
        parent = newNode->parent;
        grandparent = parent->parent;

        // Caso 1: O pai do novo nó é filho esquerdo do avô
        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;

            // Caso 1.1: O tio do novo nó é vermelho, apenas recolorir
            if (uncle != NULL && uncle->color == 1) {
                grandparent->color = 1;
                parent->color = 0;
                uncle->color = 0;
                newNode = grandparent;
            } else {
                // Caso 1.2: O tio do novo nó é preto, rotação
                if (newNode == parent->right) {
                    leftRotate(root, parent);
                    newNode = parent;
                    parent = newNode->parent;
                }
                rightRotate(root, grandparent);
                int temp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp;
                newNode = parent;
            }
        } else {  // Caso 2: O pai do novo nó é filho direito do avô
            Node* uncle = grandparent->left;

            // Caso 2.1: O tio do novo nó é vermelho, apenas recolorir
            if ((uncle != NULL) && (uncle->color == 1)) {
                grandparent->color = 1;
                parent->color = 0;
                uncle->color = 0;
                newNode = grandparent;
            } else {
                // Caso 2.2: O tio do novo nó é preto, rotação
                if (newNode == parent->left) {
                    rightRotate(root, parent);
                    newNode = parent;
                    parent = newNode->parent;
                }
                leftRotate(root, grandparent);
                int temp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp;
                newNode = parent;
            }
        }
    }

    (*root)->color = 0; // Raiz sempre é preta
}

// Função para inserir um nó na árvore
void insert(Node** root, int data) {
    // Criação do nó
    Node* newNode = createNode(data);

    // Inserção similar a uma árvore binária de busca
    Node* current = *root;
    Node* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (newNode->data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;
    if (parent == NULL)
        *root = newNode;
    else if (newNode->data < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    // Chama a função para balancear a árvore após a inserção
    fixInsertion(root, newNode);
}

// Função para buscar um nó na árvore
Node* search(Node* root, int data) {
    if (root == NULL || root->data == data)
        return root;

    if (data < root->data)
        return search(root->left, data);

    return search(root->right, data);
}

// Função para imprimir a árvore (em ordem)
void inOrderTraversal(Node* root) {
    if (root == NULL)
        return;

    inOrderTraversal(root->left);
    printf("%d ", root->data);
    inOrderTraversal(root->right);
}

int main() {
    Node* root = NULL;

    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 5);
    insert(&root, 15);
    insert(&root, 30);

    printf("Árvore Rubro-Negra (in-order): ");
    inOrderTraversal(root);
    printf("\n");

    int searchValue = 15;
    Node* result = search(root, searchValue);
    if (result != NULL)
        printf("Valor %d encontrado na árvore.\n", searchValue);
    else
        printf("Valor %d não encontrado na árvore.\n", searchValue);

    return 0;
}
