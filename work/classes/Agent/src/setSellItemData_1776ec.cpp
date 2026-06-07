#include "class.h"

struct Triple { int a, b, c; };

// target: adds r0,#0x34; stmia r0!,{r1,r2,r3}; bx lr  (returns this+0x40)
extern "C" Triple *Agent_setSellItemData(Agent *self, int a, int b, int c)
{
    Triple *p = (Triple *)((char *)self + 0x34);
    p->a = a;
    p->b = b;
    p->c = c;
    return p + 1;
}
