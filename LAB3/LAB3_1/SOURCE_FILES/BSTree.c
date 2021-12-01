#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"

//Local functions
int intmax(int a, int b);
int smallestInTree(const BSTree tree);
int largestInTree(const BSTree tree);
void populateArray(BSTree tree, int *array, int *count);
int isBalanced(const BSTree tree);

/* Creatign a treeNode with the given data by allocating memmory for the node. */
static struct treeNode *createNode(int data)
{
   //Allocating memmory for treeNode
   struct treeNode *newNode = (struct treeNode *)malloc(sizeof(struct treeNode));
   //Testing allocation
   if (newNode != NULL)
   {
      newNode->data = data;
      newNode->left = NULL;
      newNode->right = NULL;
      return newNode;
   }
   //If allocation fails we can no longer continue. Exiting with error code 1.
   printf("%s", "Memmory allocaton failed. ");
   exit(1);
}

/* Create a dynamicly allocated array containing the data of the tree sorted in ascending order. */
static int *writeSortedToArray(const BSTree tree)
{
   //Count the number of nodes to decide the size of the array.
   int size = numberOfNodes(tree);

   //Allocate memmory for the array.
   int *array = (int *)malloc(sizeof(int) * size);
   //Testing allocation
   if (array != NULL)
   {
      //Use helper function to populate the array recursivly.
      int i = 0;
      populateArray(tree, array, &i);
      return array;
   }
   //If allocation fails we can no longer continue. Exiting with error code 2.
   printf("%s", "Memmory allocaton failed. ");
   exit(2);
}

/*Recursivly popluate an array with data from the tree sorted in ascending order. Using same principle as "print in order". */
void populateArray(BSTree tree, int *array, int *count)
{
   //Traverse left
   if (tree->left != NULL)
   {
      populateArray(tree->left, array, count);
   }
   //Add node data to array
   array[(*count)++] = tree->data;

   //Traverse right
   if (tree->right != NULL)
   {
      populateArray(tree->right, array, count);
   }
}

/*Buil a sorted and balanced tree from a sorted array (ascending)*/
static void buildTreeSortedFromArray(BSTree *tree, const int arr[], int size)
{
   //Half the size rounded down.
   int half = floor(size / 2);

   //Create a new node with the data from the middle of the array.
   *tree = createNode(arr[half]);

   //If we have more that 2 nodes in the sub-array we can do another recursiv step. (at least one in each)
   if (size > 2)
   {
      buildTreeSortedFromArray(&(*tree)->left, arr, half);
      buildTreeSortedFromArray(&(*tree)->right, arr + half + 1, size - half - 1);
   }
   //if we had precisly 2 nodes in this sub-array. We can add the remaning one to the left.
   //The remaning one will allways be smaller. (2/2 = 1, arr[0] < arr[1])
   else if (size == 2)
   {
      (*tree)->left = createNode(arr[0]);
   }
}

/*Creates a empty tree. */
BSTree emptyTree(void)
{
   return NULL;
}

/*Checks if tree is empty. Returns 1 if empty otherwise 0. */
int isEmpty(const BSTree tree)
{
   return (tree == NULL);
}

/*Inserts node into tree and makes sure the tree stays sorted. */
void insertSorted(BSTree *tree, int data)
{
   //If the tree is empty, create root.
   if (isEmpty(*tree))
   {
      *tree = createNode(data);
      return;
   }

   //if the data is grater than the current data, test right node
   if (data > (*tree)->data)
   {
      //If the right node is empty, allocate node to right node.
      if ((*tree)->right == NULL)
      {
         struct treeNode *newNode = createNode(data);
         if (newNode != NULL)
         {
            (*tree)->right = newNode;
         }
      }
      //otherwise do recursiv step to the right.
      else
      {
         return insertSorted(&((*tree)->right), data);
      }
   }
   //if the data is less than the current data, test left node
   else if (data < (*tree)->data)
   {
      //If the left node is empty, allocate node to left node.
      if ((*tree)->left == NULL)
      {
         struct treeNode *newNode = createNode(data);
         if (newNode != NULL)
         {
            (*tree)->left = newNode;
         }
      }
      else
      {
         //otherwise do recursiv step to the left.
         insertSorted(&((*tree)->left), data);
      }
   }
   //assert that the node as been inserted.
   assert(find(*tree, data));
}

/*Prints tree in preorder. */
void printPreorder(const BSTree tree, FILE *textfile)
{
   fprintf(textfile, "%d ", tree->data);
   if (tree->left != NULL)
   {
      printPreorder(tree->left, textfile);
   }
   if (tree->right != NULL)
   {
      printPreorder(tree->right, textfile);
   }
}

/*Prints tree in inorder. */
void printInorder(const BSTree tree, FILE *textfile)
{
   if (tree->left != NULL)
   {
      printInorder(tree->left, textfile);
   }
   fprintf(textfile, "%d ", tree->data);
   if (tree->right != NULL)
   {
      printInorder(tree->right, textfile);
   }
}

/*Prints tree in postorder. */
void printPostorder(const BSTree tree, FILE *textfile)
{
   if (tree->left != NULL)
   {
      printPostorder(tree->left, textfile);
   }
   if (tree->right != NULL)
   {
      printPostorder(tree->right, textfile);
   }
   fprintf(textfile, "%d ", tree->data);
}

/*Checks if a node with specified data exists. */
int find(const BSTree tree, int data)
{
   //If the tree is empty, the node does not exist
   if (isEmpty(tree))
   {
      return 0;
   }

   //we found a node with matching data.
   if (tree->data == data)
      return 1;

   //if the data is less than current nodes data, do recursiv step to the left
   if (data < tree->data)
   {
      return find(tree->left, data);
   }
   //if the data is grater than current nodes data, do recursiv step to the right
   else
   {
      return find(tree->right, data);
   }
}

/*Removes a node with matching data from the tree. */
void removeElement(BSTree *tree, int data)
{
   //If the tree is empty, there is nothing to remove.
   if (isEmpty(*tree))
   {
      return;
   }

   //If the provided data is less than current nodes data, do recursiv step left.
   if ((*tree)->data > data)
   {
      removeElement(&(*tree)->left, data);
   }
   //If the provided data is grater than current nodes data, do recursiv step right.
   else if ((*tree)->data < data)
   {
      removeElement(&(*tree)->right, data);
   }
   //We found the node to remove.
   else
   {
      //if the node has 2 children, the order of these if statments are important.
      if ((*tree)->left != NULL && (*tree)->right != NULL)
      {
         //get the smallest value to the right of this ndoe.
         int nodeToRemove = smallestInTree((*tree)->right);
         //then remove that node.
         removeElement(&(*tree)->right, nodeToRemove);
         //Set the current node data to the removed nodes data
         (*tree)->data = nodeToRemove;
      }
      //If the node has one child to the left
      else if ((*tree)->left != NULL)
      {
         //Crete a new pointer for the note to be removed.
         struct treeNode *nodeToRemove = *tree;
         //set the current node to left node
         *tree = nodeToRemove->left;
         //Remove the node
         free(nodeToRemove);
         nodeToRemove = NULL;
      }
      //If the node has one child to the right
      else if ((*tree)->right != NULL)
      {
         //Crete a new pointer for the note to be removed.
         struct treeNode *nodeToRemove = *tree;
         //set the current node to right node
         (*tree) = nodeToRemove->right;
         //Remove the node
         free(nodeToRemove);
         nodeToRemove = NULL;
      }
      //The node to remove is a leaf.
      else
      {
         //Remove the node
         free(*tree);
         *tree = NULL;
      }
   }
   assert(!find(*tree, data)); //This is running multiple times
}

//Finds the node with the smallest data in the tree
int smallestInTree(const BSTree tree)
{
   if (tree->left == NULL)
   {
      return tree->data;
   }
   return smallestInTree(tree->left);
}
//Finds the node with the gratest data in the tree
int largestInTree(const BSTree tree)
{
   if (tree->right == NULL)
   {
      return tree->data;
   }
   return largestInTree(tree->right);
}

/*Returns how many nodes are in the tree. */
int numberOfNodes(const BSTree tree)
{
   //If the tree is empty, there are no nodes in the tree.
   if (isEmpty(tree))
      return 0;
   //Traverse the entire tree adding +1 at every node
   return 1 + numberOfNodes(tree->left) + numberOfNodes(tree->right);
}

/*Returns how deep the tree is*/
int depth(const BSTree tree)
{
   //If the tree is empty, the depth is 0.
   if (isEmpty(tree))
   {
      return 0;
   }
   //Traverse the entire tree, returning only the max depth of the right or the left side.
   return  intmax(depth(tree->left), depth(tree->right)) + 1;
}

/*Returns the least possible depth for the tree. */
int minDepth(const BSTree tree)
{
   return (int)ceil(log2(numberOfNodes(tree) + 1));
}

/*Balances a tree. */
void balanceTree(BSTree *tree)
{
   //If the tree is empty, don't balance it. 
   if (isEmpty(*tree))
      return;
   //If the tree is alread balanced, don't do it again. 
   if (isBalanced(*tree))
      return;
   //Count the number of nodes in the tree. 
   int size = numberOfNodes(*tree);
   //Write the tree to a sorted array. 
   int *arr = writeSortedToArray(*tree);
   //Free the tree. 
   freeTree(tree);
   //Build a tree from the serted array. 
   buildTreeSortedFromArray(tree, arr, size);
   //Free the array
   free(arr);
   arr = NULL;

   assert(size = numberOfNodes(*tree));
   assert(isBalanced(*tree));
}

int isBalanced(const BSTree tree)
{
   return (depth(tree) == minDepth(tree));
}

/*Remove the entire tree.*/
void freeTree(BSTree *tree)
{
   //Traverse the entire tree
   if ((*tree)->left != NULL)
   {
      freeTree(&(*tree)->left);
   }
   if ((*tree)->right != NULL)
   {
      freeTree(&(*tree)->right);
   }
   //Remove the node once we are on a leaf. 
   free(*tree);
   *tree = NULL;
   assert(isEmpty(*tree));
}

//Returns the grater value. 
int intmax(int a, int b)
{
   if (a > b)
      return a;
   else
      return b;
}