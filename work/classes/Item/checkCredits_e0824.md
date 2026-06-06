# Item::checkCredits
elf 0xe0824 body 32
Sig: undefined __thiscall checkCredits(Item * this)

## decompile
```c

/* Item::checkCredits() */

bool __thiscall Item::checkCredits(Item *this)

{
  int iVar1;
  
  iVar1 = Status::getCredits();
  return *(int *)(this + 0x18) <= iVar1;
}

```
## target disasm
```
  000f0824: push {r4,r6,r7,lr}
  000f0826: add r7,sp,#0x8
  000f0828: mov r4,r0
  000f082a: ldr r0,[0x000f0844]
  000f082c: add r0,pc
  000f082e: ldr r0,[r0,#0x0]
  000f0830: ldr r0,[r0,#0x0]
  000f0832: blx 0x000733d8
  000f0836: ldr r2,[r4,#0x18]
  000f0838: movs r1,#0x0
  000f083a: cmp r0,r2
  000f083c: it ge
  000f083e: mov.ge r1,#0x1
  000f0840: mov r0,r1
  000f0842: pop {r4,r6,r7,pc}
```
