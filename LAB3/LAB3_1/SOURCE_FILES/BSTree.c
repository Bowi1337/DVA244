#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"

/*Det �r helt till�tet att l�gga till egna hj�lpfunktioner men de befintliga funktionerna f�r inte �ndras*/

/* Statiska hjalpfunktioner anvands av andra funktioner i tradet och ska inte ligga i interfacet (anvandaren behover inte kanna till dessa) */

//Local functions
int max(int a, int b);
int smallestInTree(BSTree tree);
int largestInTree(BSTree tree);
void buildTree(BSTree *tree, const int arr[], int size);
void populateArray(BSTree tree, int *array, int *count);

/* Skapar en tradnod med det givna datat genom att allokera minne for noden. Glom inte att initiera pekarna*/
static struct treeNode *createNode(int data)
{
   // Glom inte att testa sa att allokeringen lyckades
   struct treeNode *newNode = (struct treeNode *)malloc(sizeof(struct treeNode));
   if (newNode != NULL)
   {
      newNode->data = data;
      newNode->left = NULL;
      newNode->right = NULL;
      return newNode;
   }
   printf("%s", "Memmory allocaton failed. ");
   return NULL;
}

/* Returnerar en dynamiskt allokerad array som innehaller tradets data sorterat */
static int *writeSortedToArray(const BSTree tree)
{
   int size = numberOfNodes(tree);
   int *array = (int *)malloc(sizeof(int) * size);
   if (array != NULL)
   {
      int i = 0;
      populateArray(tree, array, &i);
      return array;
   }
   printf("%s", "Memmory allocaton failed. ");
   return NULL;
}

/* Bygger upp ett sorterat, balanserat trad fran en sorterad array */
static void buildTreeSortedFromArray(BSTree *tree, const int arr[], int size)
{
   buildTree(tree, arr, size);
}

void buildTree(BSTree *tree, const int arr[], int size)
{
   int half = floor(size / 2);
   *tree = createNode(arr[half]);
   if (size > 2)
   {
      buildTree(&(*tree)->left, arr, half);
      buildTree(&(*tree)->right, arr + half + 1, size - half - 1);
   }
   else if (size > 1)
   {
      (*tree)->left = createNode(arr[0]);
   }
}

void populateArray(BSTree tree, int *array, int *count)
{
   if (tree->left != NULL)
   {
      populateArray(tree->left, array, count);
   }
   array[(*count)++] = tree->data;
   if (tree->right != NULL)
   {
      populateArray(tree->right, array, count);
   }
}

/* Implementation av tradet, funktionerna i interfacet */

/* Skapar ett tomt trad - denna funktion ar fardig */
BSTree emptyTree(void)
{
   return NULL;
}

/* Returnerar 1 ifall tradet ar tomt, 0 annars */
int isEmpty(const BSTree tree)
{
   if (tree == NULL)
      return 1;
   return 0;
}

/* Satter in 'data' sorterat i *tree
 Post-condition: data finns i tradet*/
void insertSorted(BSTree *tree, int data)
{
   if (*tree == NULL)
   {
      *tree = createNode(data);
      return;
   }
   if (data > (*tree)->data)
   {
      if ((*tree)->right == NULL)
      {
         struct treeNode *newNode = createNode(data);
         if (newNode != NULL)
         {
            (*tree)->right = newNode;
         }
      }
      else
      {
         return insertSorted(&((*tree)->right), data);
      }
   }
   else if (data < (*tree)->data)
   {
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
         insertSorted(&((*tree)->left), data);
      }
   }
   assert(find(*tree, data));
}

/* Utskriftsfunktioner
   Vid anrop: anvand stdio som andra argument for att skriva ut p� skarmen
   Det racker att ni implementerar LR ordningarna*/
void printPreorder(const BSTree tree, FILE *textfile)
{
   fprintf(textfile, "%d\n", tree->data);
   if (tree->left != NULL)
   {
      printInorder(tree->left, textfile);
   }
   if (tree->right != NULL)
   {
      printInorder(tree->right, textfile);
   }
}

void printInorder(const BSTree tree, FILE *textfile)
{
   if (tree->left != NULL)
   {
      printInorder(tree->left, textfile);
   }
   fprintf(textfile, "%d\n", tree->data);
   if (tree->right != NULL)
   {
      printInorder(tree->right, textfile);
   }
}

void printPostorder(const BSTree tree, FILE *textfile)
{
   if (tree->left != NULL)
   {
      printInorder(tree->left, textfile);
   }
   if (tree->right != NULL)
   {
      printInorder(tree->right, textfile);
   }
   fprintf(textfile, "%d\n", tree->data);
}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
   if (tree == NULL)
      return 0;

   if (tree->data == data)
      return 1;

   if (data < tree->data)
   {
      return find(tree->left, data);
   }
   else
   {
      return find(tree->right, data);
   }
}

/* Tar bort 'data' fran tradet om det finns */
void removeElement(BSTree *tree, int data)
{
   if (*tree == NULL)
   {
      return;
   }
   if ((*tree)->data > data)
   {
      removeElement(&(*tree)->left, data);
   }
   else if ((*tree)->data < data)
   {
      removeElement(&(*tree)->right, data);
   }
   else
   {
      if ((*tree)->left != NULL && (*tree)->right != NULL)
      {
         int nodeToRemove = smallestInTree((*tree)->right);
         removeElement(tree, nodeToRemove);
         (*tree)->data = nodeToRemove;
      }
      else if ((*tree)->left != NULL)
      {
         struct treeNode *nodeToRemove = (*tree);
         (*tree) = nodeToRemove->left;
         free(nodeToRemove);
         nodeToRemove = NULL;
      }
      else if ((*tree)->right != NULL)
      {
         struct treeNode *nodeToRemove = (*tree);
         (*tree) = nodeToRemove->right;
         free(nodeToRemove);
         nodeToRemove = NULL;
      }
      else
      {
         free(*tree);
         *tree = NULL;
      }
   }
   assert(!find(*tree, data));//This is running multiple times
}

int smallestInTree(BSTree tree)
{
   if (tree->left == NULL)
   {
      return tree->data;
   }
   else
   {
      return smallestInTree(tree->left);
   }
}

int largestInTree(BSTree tree)
{
   if (tree->right == NULL)
   {
      return tree->data;
   }
   else
   {
      return largestInTree(tree->right);
   }
}

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree)
{
   if (tree == NULL)
      return 0;
   int count = 1;
   if (tree->left != NULL)
   {
      count += numberOfNodes(tree->left);
   }
   if (tree->right != NULL)
   {
      count += numberOfNodes(tree->right);
   }
   return count;
}

/* Returnerar hur djupt tradet ar */
int depth(const BSTree tree)
{
   if (tree == NULL)
   {
      return 0;
   }
   int leftMax = 0;
   int rightMax = 0;
   if (tree->left != NULL)
   {
      leftMax = depth(tree->left);
   }
   if (tree->right != NULL)
   {
      rightMax = depth(tree->right);
   }
   return max(leftMax, rightMax) + 1;
}

/* Returnerar minimidjupet for tradet
   Se math.h for anvandbara funktioner*/
int minDepth(const BSTree tree)
{
   return (int)ceil(log2(numberOfNodes(tree) + 1));
}

/* Balansera tradet sa att depth(tree) == minDepth(tree) */
void balanceTree(BSTree *tree)
{
   if (*tree == NULL)
      return;
   int size = numberOfNodes(*tree);
   int *arr = writeSortedToArray(*tree);
   freeTree(tree);
   buildTreeSortedFromArray(tree, arr, size);
   free(arr);
   arr = NULL;
   assert(size = numberOfNodes(*tree));
   assert(minDepth(*tree) == depth(*tree));
}

/* Tom tradet och frigor minnet for de olika noderna */
void freeTree(BSTree *tree)
{
   // Post-condition: tradet ar tomt
   if ((*tree)->left != NULL)
   {
      freeTree(&(*tree)->left);
   }
   if ((*tree)->right != NULL)
   {
      freeTree(&(*tree)->right);
   }
   free(*tree);
   *tree = NULL;
   assert(isEmpty(*tree));//This is running multiple times
}

int max(int a, int b)
{
   if (a > b)
      return a;
   else
      return b;
}