#include "queue.h"
#include<assert.h>

/*Har ar det viktigt att ateranvanda koden som du redan skrivit for den lankade listan.
  Ateranvandning betyder inte att man kopierar koden eller skriver av den, ateranvander
  gor man genom att anropa redan implementerade funktioner.*/

/*Funktionen ar fardig*/
Queue initializeQueue(void)
{
    return NULL;
}

int queueIsEmpty(const Queue queue)
{
    return isEmpty(queue);	// Ersatt med ratt returvarde
}

/*Postcondition: data ar tillagt sist i kon*/
void enqueue(Queue* queue, const Data data)
{
    addLast(queue, data);
    assert(getLastElement(*queue) == data);
}

/* Precondition: kon �r ej tom */
void dequeue(Queue* queue)
{
    assert(*queue != NULL);
    removeFirst(queue);
}

/* Precondition: kon �r ej tom */
Data peekQueue(const Queue queue)
{
    assert(queue != NULL);
    return getFirstElement(queue);	// Ersatt med ratt returvarde
}


/* Anvands for testning och felsokning
   Tips: det kan vara bra att ocksa notera i utskriften vart borjan respektive slutet p� kon �r */
void printQueue(const Queue queue, FILE *textfile)
{
    printList(queue, textfile);
}


