# Array<ListItem*>::Array
elf 0x11dde8 body 28
Sig: undefined __thiscall Array(Array<ListItem*> * this)

## decompile
```c

/* Array<ListItem*>::Array() */

Array<ListItem*> * __thiscall Array<ListItem*>::Array(Array<ListItem*> *this)

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
  0012dde8: push {r4,r6,r7,lr}
  0012ddea: add r7,sp,#0x8
  0012ddec: mov r4,r0
  0012ddee: movs r0,#0x4
  0012ddf0: blx 0x0006ec08
  0012ddf4: movs r2,#0x0
  0012ddf6: movs r1,#0x1
  0012ddf8: str r2,[r0,#0x0]
  0012ddfa: strd r2,r0,[r4,#0x0]
  0012ddfe: mov r0,r4
  0012de00: str r1,[r4,#0x8]
  0012de02: pop {r4,r6,r7,pc}
```
