#include "list.h"
#include <stdlib.h>
#include <assert.h>

/*Det �r helt till�tet att l�gga till egna hj�lpfunktioner men de befintliga funktionerna f�r inte �ndras*/

/*Hjalpfunktion till add
  Allokerar minne for en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (eller NULL) returneras.*/
static struct node *createListNode(const Data data)
{
  //Glom inte att testa sa att allokeringen lyckades innan du initierar noden

  //Allocate memmory for one node
  struct node *newNode = (struct node *)malloc(sizeof(struct node));
  if (newNode != NULL)
  {
    //Assign data to node
    newNode->data = data;
    //return new node
    return newNode;
  }
  printf("%s", "Memmory allocation failed.");
  return NULL;
}

/*Returnera en tom lista - funktionen ar fardig*/
List createEmptyList(void)
{
  return NULL;
}

/*Ar listan tom?
  Returnerar 1 om den �r tom, annars 0*/
int isEmpty(const List list)
{
  //If the head is NULL => list is empty
  if (list == NULL)
  {
    return 1;
  }
  return 0;
}

/*Lagg till nod forst i listan*/
/*Postcondition: Det nya datat ligger forst i listan (testa med assert)*/
void addFirst(List *list, const Data data)
{
  //Create new node to work with
  struct node *newNode = createListNode(data);
  if (newNode != NULL)
  {
    //If the list is empty, replace head with node.
    if (*list == NULL)
    {
      *list = newNode;
      assert(data == (*list)->data);
      return;
    }
    //Set the head to a temporary variable
    struct node *temp = *list;

    //set the head to the new node
    *list = newNode;

    //link the first node to the new first node
    newNode->next = temp;
    newNode->next->previous = newNode;
    assert(data == (*list)->data);
  }
  //Anropa createListNode for att skapa den nya noden
  //Glom inte att testa att den nya noden faktiskt kunde skapas/tilldelas minne innan du fortsatter
  //Tank pa att listan kan vara tom nar en ny nod laggs till
}

/*Lagg till nod sist i listan
  Tips, nar du hittat ratt plats kan du anvanda funktionen addFirst for att lagga till*/
void addLast(List *list, const Data data)
{
  //Create new node to work with
  struct node *newNode = createListNode(data);
  if (newNode != NULL)
  {
    //If the list is empty, replace head with node.
    if (isEmpty(*list))
    {
      *list = newNode;
      assert((*list)->data == data);
      return;
    }
    //Loop untill last node's next node is null => we are on the last node.
    struct node *lastNode;
    for (lastNode = *list; lastNode->next != NULL; lastNode = lastNode->next)
      ;
    
    //link the last node with the new last node
    lastNode->next = newNode;
    newNode->previous = lastNode;
    assert(getLastElement(*list) == data);
  }
}

/*Ta bort forsta noden i listan
  Precondition: listan ar inte tom (testa med assert)
  Noden ska lankas ur och minnet frigoras, resten av listan ska finnas kvar*/
void removeFirst(List *list)
{
  assert(!isEmpty(*list));
  //Glom inte att frigora minnet for den nod som lankas ur listan.
  //Tank pa att listans huvud efter bortlankningen maste peka pa den nod som nu ar forst.

  //If there is only one node => remove it
  if ((*list)->next == NULL)
  {
    free(*list);
    *list = NULL;
    return;
  }

  //create new temporary node ponter
  struct node *temp = *list;

  //Set the new head to next node
  *list = (*list)->next;

  //Remove the node
  free(temp);
  (*list)->previous = NULL;
}

/*Ta bort sista noden i listan
  Precondition: listan ar inte tom (testa med assert)t*/
void removeLast(List *list)
{
  assert(!isEmpty(*list));
  //Glom inte att frigora minnet for den nod som lankas ur listan.
  //Tank pa att den nod som nu ar sist inte pekar nagonstans, dess pekare maste nollstallas

  //If there is only one node => remove it
  if ((*list)->next == NULL)
  {
    free(*list);
    *list = NULL;
    return;
  }
    //Loop untill the node two nodes down is null => we are on the second last node.
  struct node *secondLastNode;
  for (secondLastNode = *list; secondLastNode->next->next != NULL; secondLastNode = secondLastNode->next)
    ;
  //remove the second last next node
  free(secondLastNode->next);
  secondLastNode->next = NULL;
  
}

/*Ta bort data ur listan (forsta forekomsten)
  Returnera 1 om datat finns, annars 0
  Tips, nar du hittar ratt nod kan du anvanda en av de ovanstaende funktionerna for att ta bort noden*/
int removeElement(List *list, const Data data)
{
  //If the list is empty, there is nothing to remove
  if (isEmpty(*list))
  {
    return 0;
  }
  //Loop untill node->data is equal to data or we are on the last node
  struct node *nodeToRemove;
  for (nodeToRemove = *list; nodeToRemove->data != data && nodeToRemove->next != NULL; nodeToRemove = nodeToRemove->next)
    ;

  //If the node->data we landed on is not equal to data => we culd not find the node
  if (nodeToRemove->data != data)
  {
    return 0;
  }
  //If it is not the first node link the previous node
  if (nodeToRemove->previous != NULL)
  {
    nodeToRemove->previous->next = nodeToRemove->next;
  }
  //If it is the first node, link the next node to the head. 
  else
  {
    *list = nodeToRemove->next;
  }
  //If it is not the last node, link the next node to the previous
  if (nodeToRemove->next != NULL)
  {
    nodeToRemove->next->previous = nodeToRemove->previous;
  }
  //After all the relinking is done, remove the node
  free(nodeToRemove);
  return 1; //Ersatt med ratt returvarde
}

/*Finns data i listan?
  Om datat finns returneras 1, annars 0
  Tank pa att listan kan vara tom*/
int search(const List list, const Data data)
{
  //If the list is empty => we cant find the node
  if (list == NULL)
    return 0;

  //Loop untill node->data is equal to data or we are on the last node
  struct node *nodeToFind;
  for (nodeToFind = list; nodeToFind->data != data && nodeToFind->next != NULL; nodeToFind = nodeToFind->next)
    ;
  //if the node->data we stoped on equals data => we found the node
  if (nodeToFind->data == data)
  {
    return 1;
  }
  return 0;
}

/*Returnera antalet noder i listan*/
int numberOfNodesInList(const List list)
{
  int count = 0;
  //loop thrugh the entire list incrementing 'count' every loop
  //This also works for empty lists
  for (struct node *node = list; node != NULL; node = node->next)
  {
    count++;
  }
  return count;
}

/*Ta bort alla noder ur listan
  Glom inte att frigora minnet
  Postcondition: Listan ar tom, *list �r NULL (testa med assert)*/
void clearList(List *list)
{
  if (isEmpty(*list))
  {
    return;
  }
  //Alla noder maste tas avallokeras en och en, det racker inte att endast frigora list.

  //If there is only one node => remove it
  if ((*list)->next == NULL)
  {
    free(*list);
    *list = NULL;
    assert(isEmpty(*list));
    return;
  }
  //loop thrugh the entire list starting at the second node and stopping at the last node.
  //in every loop => remove the previous node
  struct node *temp;
  for (temp = (*list)->next; temp->next != NULL; temp = temp->next)
  {
    free(temp->previous);
  }
  //since we stoped att the last node, we did not remove the second last node
  //remove the last node and second last node;
  free(temp->previous);
  free(temp);
  *list = NULL;

  assert(isEmpty(*list));
}

/*Skriv ut listan
  Vid anropet kan man ange stdout som argument 2 for att skriva ut p� skarmen.
  Anvanda fprintf for att skriva ut.
  Den har typen av utskriftfunktion blir mer generell da man kan valja att skriva ut till skarmen eller till fil.*/
void printList(const List list, FILE *textfile)
{
  //Print array like [1, 2, 3, 4, 5, ...]
  fprintf(textfile, "%s", "[");
  struct node *node;
  for (node = list; node->next != NULL; node = node->next)
  {
    fprintf(textfile, "%d, ", node->data);
  }
  fprintf(textfile, "%d]\r\n", node->data);
}

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getFirstElement(const List list)
{
  assert(!isEmpty(list));
  //return first element data
  return list->data;
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getLastElement(const List list)
{
  assert(!isEmpty(list));
  //Loop untill last node
  struct node *lastNode;
  for (lastNode = list; lastNode->next != NULL; lastNode = lastNode->next)
    ;
  //return last node data
  return lastNode->data; //Ersatt med ratt returvarde
}