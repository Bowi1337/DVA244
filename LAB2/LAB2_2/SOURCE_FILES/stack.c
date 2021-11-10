#include "stack.h"
#include<assert.h>

/*Funktionen ar fardig*/
Stack initializeStack(void)
{
    return createEmptyList();
}

int stackIsEmpty(const Stack stack)
{
    return isEmpty(stack);
}

/* Postcondition 'data' ligger overst p� stacken */
void push(Stack* stack, const Data data)
{
    addFirst(stack, data);
    assert((*stack)->data == data);
}

/* Precondition: stacken far inte vara tom */
void pop(Stack* stack)
{
    assert(*stack != NULL);
    removeFirst(stack);
}

/* Precondition: stacken far inte vara tom */
Data peekStack(const Stack stack)
{
    assert(stack != NULL);
     return getFirstElement(stack); // Ersatt denna rad med ratt returvarde
}

/* Anvands for testning och felsokning
 Tips: det kan vara bra att ocksa notera i urskriften vart toppen pa stacken ar */
void printStack(const Stack stack, FILE *textFile)
{
    printList(stack, textFile);
}
