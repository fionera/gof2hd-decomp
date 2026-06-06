# Item::equals
elf 0xe07a6 body 20
Sig: undefined __thiscall equals(Item * this, Item * param_1)

## decompile
```c

/* Item::equals(Item*) */

bool __thiscall Item::equals(Item *this,Item *param_1)

{
  if (param_1 != (Item *)0x0) {
    return *(int *)this == *(int *)param_1;
  }
  return false;
}

```
## target disasm
```
  000f07a6: cbz r1,0x000f07b6
  000f07a8: ldr r1,[r1,#0x0]
  000f07aa: ldr r0,[r0,#0x0]
  000f07ac: subs r0,r0,r1
  000f07ae: clz r0,r0
  000f07b2: lsrs r0,r0,#0x5
  000f07b4: bx lr
  000f07b6: movs r0,#0x0
  000f07b8: bx lr
```
