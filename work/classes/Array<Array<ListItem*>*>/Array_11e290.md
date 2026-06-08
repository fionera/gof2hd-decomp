# Array<Array<ListItem*>*>::Array
elf 0x11e290 body 28
Sig: undefined __thiscall Array(Array<Array<ListItem*>*> * this)

## decompile
```c

/* Array<Array<ListItem*>*>::Array() */

Array<Array<ListItem*>*> * __thiscall
Array<Array<ListItem*>*>::Array(Array<Array<ListItem*>*> *this)

{
  undefined4 *puVar1;
  
  puVar1 = operator_new__(4);
  *puVar1 = 0;
  *(undefined4 *)this = 0;
  *(undefined4 **)(this + 4) = puVar1;
  *(undefined4 *)(this + 8) = 1;
  return this;
}

```

## target disasm
```
  0012e290: push {r4,r6,r7,lr}
  0012e292: add r7,sp,#0x8
  0012e294: mov r4,r0
  0012e296: movs r0,#0x4
  0012e298: blx 0x0006ec08
  0012e29c: movs r2,#0x0
  0012e29e: movs r1,#0x1
  0012e2a0: str r2,[r0,#0x0]
  0012e2a2: strd r2,r0,[r4,#0x0]
  0012e2a6: mov r0,r4
  0012e2a8: str r1,[r4,#0x8]
  0012e2aa: pop {r4,r6,r7,pc}
```
