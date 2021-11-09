#include <stdio.h>
#include <assert.h>

#define DEBUGTEXT 0

int sum(int i)
{
    if(DEBUGTEXT)
        printf("sum(%d): called\r\n", i);
    int x = i;
    if (i > 1)
    {
        x += sum(i - 1);
    }
    else if( i < 0){
        x = 0;
    }
    if(DEBUGTEXT)
        printf("sum(%d): returned: %d\r\n", i, x);
    return x;
}

int main(void)
{
    assert(sum(1) == 1);
    assert(sum(2) == 3);
    assert(sum(3) == 6);
    assert(sum(4) == 10);
    assert(sum(5) == 15);
    assert(sum(20) == 210);
    assert(sum(0) == 0);
    assert(sum(-1) == 0);

    printf("%s\r\n", "Test compleeted");
}