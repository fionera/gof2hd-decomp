# Station::isDiscovered
elf 0xa6c10 body 24
Sig: undefined __thiscall isDiscovered(Station * this)

## decompile
```c

/* Station::isDiscovered() */

undefined1 __thiscall Station::isDiscovered(Station *this)

{
  int iVar1;
  
  iVar1 = Galaxy::getVisited((Galaxy *)**(undefined4 **)(DAT_000b6c28 + 0xb6c1c));
  return *(undefined1 *)(iVar1 + *(int *)(this + 0xc));
}

```

## target disasm
```
  000b6c10: push {r4,r6,r7,lr}
  000b6c12: add r7,sp,#0x8
  000b6c14: mov r4,r0
  000b6c16: ldr r0,[0x000b6c28]
  000b6c18: add r0,pc
  000b6c1a: ldr r0,[r0,#0x0]
  000b6c1c: ldr r0,[r0,#0x0]
  000b6c1e: blx 0x00073480
  000b6c22: ldr r1,[r4,#0xc]
  000b6c24: ldrb r0,[r0,r1]
  000b6c26: pop {r4,r6,r7,pc}
```
