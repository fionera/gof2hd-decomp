# ListItem::~ListItem
elf 0xa6748 body 38
Sig: undefined __thiscall ~ListItem(ListItem * this)

## decompile
```c

/* ListItem::~ListItem() */

ListItem * __thiscall ListItem::~ListItem(ListItem *this)

{
  if (*(int **)(this + 0x1c) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x1c) + 4))();
    *(undefined4 *)(this + 0x1c) = 0;
  }
  if (*(int **)(this + 0x20) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x20) + 4))();
    *(undefined4 *)(this + 0x20) = 0;
  }
  return this;
}

```

## target disasm
```
  000b6748: push {r4,r6,r7,lr}
  000b674a: add r7,sp,#0x8
  000b674c: mov r4,r0
  000b674e: ldr r0,[r0,#0x1c]
  000b6750: cbz r0,0x000b675c
  000b6752: ldr r1,[r0,#0x0]
  000b6754: ldr r1,[r1,#0x4]
  000b6756: blx r1
  000b6758: movs r0,#0x0
  000b675a: str r0,[r4,#0x1c]
  000b675c: ldr r0,[r4,#0x20]
  000b675e: cbz r0,0x000b676a
  000b6760: ldr r1,[r0,#0x0]
  000b6762: ldr r1,[r1,#0x4]
  000b6764: blx r1
  000b6766: movs r0,#0x0
  000b6768: str r0,[r4,#0x20]
  000b676a: mov r0,r4
  000b676c: pop {r4,r6,r7,pc}
```
