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
  struct node *newNode = (struct node *)malloc(sizeof(struct node));
  if (newNode != NULL)
  {
    newNode->data = data;
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
  if (list == NULL)
  {
    return 1;
  }
  return 0; //ersatt med ratt returvarde
}

/*Lagg till nod forst i listan*/
/*Postcondition: Det nya datat ligger forst i listan (testa med assert)*/
void addFirst(List *list, const Data data)
{
  struct node *newNode = createListNode(data);
  if (newNode != NULL)
  {
    if (*list == NULL)
    {
      *list = newNode;
    }
    else
    {
      struct node *temp = *list;
      *list = newNode;
      newNode->next = temp;
      newNode->next->previous = newNode;
    }
  }
  //Anropa createListNode for att skapa den nya noden
  //Glom inte att testa att den nya noden faktiskt kunde skapas/tilldelas minne innan du fortsatter
  //Tank pa att listan kan vara tom nar en ny nod laggs till
}

/*Lagg till nod sist i listan
  Tips, nar du hittat ratt plats kan du anvanda funktionen addFirst for att lagga till*/
void addLast(List *list, const Data data)
{
  struct node *newNode = createListNode(data);
  if (newNode != NULL)
  {
    if (*list == NULL)
    {
      *list = newNode;
    }
    else
    {
      struct node *lastNode;
      for (lastNode = *list; lastNode->next != NULL; lastNode = lastNode->next)
        ;
      lastNode->next = newNode;
      newNode->previous = lastNode;
    }
  }
}

/*Ta bort forsta noden i listan
  Precondition: listan ar inte tom (testa med assert)
  Noden ska lankas ur och minnet frigoras, resten av listan ska finnas kvar*/
void removeFirst(List *list)
{
  assert(*list != NULL);
  //Glom inte att frigora minnet for den nod som lankas ur listan.
  //Tank pa att listans huvud efter bortlankningen maste peka pa den nod som nu ar forst.
  if ((*list)->next == NULL)
  {
    free(*list);
    *list = NULL;
  }
  else
  {
    struct node *temp = *list;
    *list = (*list)->next;
    free(temp);
    (*list)->previous = NULL;
  }
}

/*Ta bort sista noden i listan
  Precondition: listan ar inte tom (testa med assert)t*/
void removeLast(List *list)
{
  assert(*list != NULL);
  //Glom inte att frigora minnet for den nod som lankas ur listan.
  //Tank pa att den nod som nu ar sist inte pekar nagonstans, dess pekare maste nollstallas
  if ((*list)->next == NULL)
  {
    free(*list);
    *list = NULL;
  }
  else
  {
    struct node *lastNode;
    for (lastNode = *list; lastNode->next != NULL; lastNode = lastNode->next)
      ;
    struct node *secondLastNode = lastNode->previous;
    free(lastNode);
    secondLastNode->next = NULL;
  }
}

/*Ta bort data ur listan (forsta forekomsten)
  Returnera 1 om datat finns, annars 0
  Tips, nar du hittar ratt nod kan du anvanda en av de ovanstaende funktionerna for att ta bort noden*/
int removeElement(List *list, const Data data)
{
  if(*list == NULL){
    return 0;
  }
  struct node *nodeToRemove;
  for (nodeToRemove = *list; nodeToRemove->data != data && nodeToRemove->next != NULL; nodeToRemove = nodeToRemove->next){
    
  }
    ;
  if (nodeToRemove->data != data)
  {
    return 0;
  }
  if(nodeToRemove->previous != NULL){
  nodeToRemove->previous->next = nodeToRemove->next;
  }
  else{
    *list = nodeToRemove->next;
  }
  if(nodeToRemove->next != NULL){
    nodeToRemove->next->previous = nodeToRemove->previous;
  }
  free(nodeToRemove);
  return 1; //Ersatt med ratt returvarde
}

/*Finns data i listan?
  Om datat finns returneras 1, annars 0
  Tank pa att listan kan vara tom*/
int search(const List list, const Data data)
{
  if (list == NULL)
    return 0;

  struct node *nodeToFind;
  for (nodeToFind = list; nodeToFind->data != data && nodeToFind->next != NULL; nodeToFind = nodeToFind->next)
    ;
  if (nodeToFind->data == data)
  {
    return 1;
  }
  return 0;
}

/*Returnera antalet noder i listan*/
int numberOfNodesInList(const List list)
{
  if (list == NULL)
  {
    return 0;
  }
  int count = 0;

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
  if(*list == NULL){
    return;
  }
  //Alla noder maste tas avallokeras en och en, det racker inte att endast frigora list.
  if ((*list)->next == NULL)
  {
    free(*list);
    *list = NULL;
    return;
  }

  struct node *temp;
  for (temp = (*list)->next; temp->next != NULL; temp = temp->next)
  {
    free(temp->previous);
  }
  free(temp->previous);
  free(temp);
  *list = NULL;

  assert((*list) == NULL);
}

/*Skriv ut listan
  Vid anropet kan man ange stdout som argument 2 for att skriva ut p� skarmen.
  Anvanda fprintf for att skriva ut.
  Den har typen av utskriftfunktion blir mer generell da man kan valja att skriva ut till skarmen eller till fil.*/
void printList(const List list, FILE *textfile)
{
}

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getFirstElement(const List list)
{
  assert(list != NULL);
  return list->data;
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getLastElement(const List list)
{
  assert(list != NULL);
  struct node *lastNode;
  for (lastNode = list; lastNode->next != NULL; lastNode = lastNode->next)
    ;
  return lastNode->data; //Ersatt med ratt returvarde
}
