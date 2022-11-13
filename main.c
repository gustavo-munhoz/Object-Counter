#include <stdio.h>
#include "stdlib.h"
#include "string.h"

/*
 * PPM Image Object Counter
 * Created by Gustavo Munhoz Corrêa
*/


typedef struct Node {
    int i, j; // coordinates of the Node
    struct Node *nextNode; // pointer to the nextNode Node of the stack
} Node;

typedef struct {
    int size;
    Node *top; // pointer to the top Node of the stack
} Stack;

typedef struct {
    int isObject, status; // 0 is not visited, 1 is visited
} Pixel;

void initStack(Stack *stack);
void push(Stack *stack, int pixel_i, int pixel_j);
Node pop(Stack *stack);
void popAll(Stack *stack);
void printStack(Stack *stack);
void readNeighbours(Stack *stack, int i, int j, int rows, int cols, Pixel **matrix);

int main(int argc, char **argv) {
    char file[] = "..\\Images\\";
    strcat(file, argv[1]);

    FILE *fp;
    Stack objectStack;

    int printImage = atoi(argv[2]);
    char id[3];
    int cols, rows, max;
    unsigned char r, g, b;
    unsigned char br, bg, bb;
    int position;
    int objectCount = 0;

    fp = fopen(file, "rb");

    if (fp == NULL) {
        printf("File not found.");
        exit(EXIT_FAILURE);
    }

    // reads header of ppm file
    fscanf(fp, "%s", id);
    fscanf(fp, "%d", &cols);
    fscanf(fp, "%d", &rows);
    fscanf(fp, "%d", &max);
    fscanf(fp, "%c", &r);

    position = ftell(fp);
    fscanf(fp, "%c%c%c", &br, &bg, &bb);
    fseek(fp, position, SEEK_SET);

    // allocates memory dinamically depending on the size of image
    Pixel **img_matrix = (Pixel **)calloc(rows, sizeof(Pixel*));
    for(int i = 0; i < rows; i++) img_matrix[i] = (Pixel *)calloc(cols, sizeof(Pixel));


    // fill matrix with 0's and 1's depending on value of pixel

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(fp, "%c%c%c", &r, &g, &b);
            if (r != br || g != bg || b != bb) {
                img_matrix[i][j].isObject = 1;
                if (printImage) printf("*");
            } else {
                if (printImage) printf(".");
            }
        }
        if (printImage) printf("\n");
    }

    initStack(&objectStack);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Pixel *currentPixel = &img_matrix[i][j];

            // checks for seed and counts object if true
            if (currentPixel -> isObject && currentPixel -> status != 1) {
                push(&objectStack, i, j);
                currentPixel -> status = 1;

                while (objectStack.size != 0) {
                    Node temp = pop(&objectStack);
                    readNeighbours(&objectStack, temp.i, temp.j, rows, cols, img_matrix);
                }
                objectCount++;
            }
        }
    }

    printf("\nObjects counted: %d", objectCount);

    return 0;
}

/**
 * Initiates an empty instance of stack.
 * @param stack address of desired Stack.
 */
void initStack(Stack *stack) {
    stack -> size = 0;
    stack -> top = NULL;
}

/**
 * Inserts data as a node on top of stack.
 * Creates the first node if the stack is empty, which points to NULL.
 * @param stack address of desired stack.
 * @param pixel_i line of pushed pixel.
 * @param pixel_j column of pushed pixel.
 */
void push(Stack *stack, int pixel_i, int pixel_j) {
    Node *nodeToPush = (Node *) malloc(sizeof (Node));
    nodeToPush -> i = pixel_i;
    nodeToPush -> j = pixel_j;

    if (stack -> top == NULL) {
        nodeToPush -> nextNode = NULL;
        stack -> top = nodeToPush;

    } else {
        nodeToPush -> nextNode = stack -> top;
        stack -> top = nodeToPush;
    }
    stack -> size++;
}

/**
 * Removes the top node of the stack and frees its allocated memory.
 * @param stack address of desired stack.
 * @return a copy of the removed Node.
 * @return a NULL stack if stack is empty.
 */
Node pop(Stack *stack) {
    Node copy;
    if (stack -> top != NULL) {
        Node *temp = stack -> top;
        stack -> top = stack -> top -> nextNode;

        copy = *temp;

        free(temp);
        stack -> size--;

        return copy;
    } else {
        copy.nextNode = NULL;
        return copy;
    }
}

/**
 * Removes all nodes of stack.
 * @param stack address of desired stack.
 */
void popAll(Stack *stack) {
    while (stack -> top != NULL) {
        pop(stack);
    }
}

/**
 * Prints all values of stack, formatted with lines and columns.
 * @param stack address of desired stack.
 */
void printStack(Stack *stack) {
    Node *aux = stack -> top;

    if (aux == NULL) printf("Empty is stack");
    else {
        while (aux != NULL) {
            printf("I: %d J: %d\n", aux -> i, aux -> j);
            aux = aux -> nextNode;
        }
    }
}

/**
 * Analyzes top, bottom, left and right pixels relative to the provided pixel coordinates.
 * If the analyzed pixel is not background pixel, pushes it to the object stack and changes its status to visited (1).
 * @param stack address of desired stack.
 * @param i line of pixel to be analyzed.
 * @param j column of pixel to be analyzed.
 * @param rows number of rows in the image matrix.
 * @param cols number of columns in the image matrix.
 * @param matrix image matrix, contains Pixel structs as elements.
 */
void readNeighbours(Stack *stack, int i, int j, int rows, int cols, Pixel **matrix) {
    if (i - 1 >= 0) {
        if (matrix[i - 1][j].isObject == 1 && matrix[i - 1][j].status != 1) {
            push(stack, i - 1, j);
            matrix[i - 1][j].status = 1;
        }
    }
    if (j - 1 >= 0) {
        if (matrix[i][j - 1].isObject == 1 && matrix[i][j - 1].status != 1) {
            push(stack, i, j - 1);
            matrix[i][j - 1].status = 1;
        }
    }
    if (i + 1 < rows) {
        if (matrix[i + 1][j].isObject == 1 && matrix[i + 1][j].status != 1) {
            push(stack, i + 1, j);
            matrix[i + 1][j].status = 1;
        }
    }
    if (j + 1 < cols) {
        if (matrix[i][j + 1].isObject == 1 && matrix[i][j + 1].status != 1) {
            push(stack, i, j + 1);
            matrix[i][j + 1].status = 1;
        }
    }
}