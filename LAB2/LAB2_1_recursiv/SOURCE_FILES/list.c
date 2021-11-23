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
  if (isEmpty(*list))
  {
    addFirst(list, data);
    return;
  }
  if ((*list)->next != NULL)
  {
    addLast(&((*list)->next), data);
    return;
  }
  //Create new node to work with
  struct node *newNode = createListNode(data);
  if (newNode != NULL)
  {
    //link the last node with the new last node
    (*list)->next = newNode;
    newNode->previous = (*list);
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

  if ((*list)->next == NULL)
  {
    removeFirst(list);
    return;
  }
  if ((*list)->next->next != NULL)
  {
    removeLast(&((*list)->next));
    return;
  }
  free((*list)->next);
  (*list)->next = NULL;
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
  if ((*list)->data != data && (*list)->next != NULL)
  {
    return removeElement(&((*list)->next), data);
  }
  //If the node->data we landed on is not equal to data => we could not find the node
  if ((*list)->data != data)
  {
    return 0;
  }
  struct node *nodeToRemove = (*list);
  //If it is not the first node link the previous node
  if (nodeToRemove->previous != NULL)
  {
    nodeToRemove->previous->next = nodeToRemove->next;
  }
  //If it is the first node
  else
  {
    removeFirst(list);
    return 1;
  }
  //If it is not the last node, link the next node to the previous
  if (nodeToRemove->next != NULL)
  {
    nodeToRemove->next->previous = nodeToRemove->previous;
  }
  //After all the relinking is done, remove the node
  free((nodeToRemove));
  nodeToRemove = NULL;
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
  if (list->data != data && list->next != NULL)
  {
    return search(list->next, data);
  }
  //if the node->data we stoped on equals data => we found the node
  if (list->data == data)
  {
    return 1;
  }
  return 0;
}

/*Returnera antalet noder i listan*/
int numberOfNodesInList(const List list)
{
  if (list == NULL)
    return 0;
  else
    return 1 + numberOfNodesInList(list->next);
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
  clearList(&((*list)->next));
  free(*list);
  *list = NULL;
  return;
  assert(isEmpty(*list));
}

/*Skriv ut listan
  Vid anropet kan man ange stdout som argument 2 for att skriva ut p� skarmen.
  Anvanda fprintf for att skriva ut.
  Den har typen av utskriftfunktion blir mer generell da man kan valja att skriva ut till skarmen eller till fil.*/
void printList(const List list, FILE *textfile)
{
  //Print array like [1, 2, 3, 4, 5, ...]
  if (list->previous == NULL)
    fprintf(textfile, "%s", "[");

  if (list->next != NULL)
  {
    fprintf(textfile, "%d, ", list->data);
    printList(list->next, textfile);
    return;
  }
  if (list->next == NULL)
    fprintf(textfile, "%d]\r\n", list->data);
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
  if (list->next == NULL)
  {
    return list->data;
  }
  else
  {
    return getLastElement(list->next);
  }
}