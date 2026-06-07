# Station::isAttackedByAliens
elf 0xa6bec body 24
Sig: undefined __thiscall isAttackedByAliens(Station * this)

## decompile
```c

/* Station::isAttackedByAliens() */

bool __thiscall Station::isAttackedByAliens(Station *this)

{
  return *(int *)(this + 0xc) == *(int *)(**(int **)(DAT_000b6c04 + 0xb6bf4) + 0x80);
}

```

## target disasm
```
  000b6bec: ldr r1,[0x000b6c04]
  000b6bee: ldr r0,[r0,#0xc]
  000b6bf0: add r1,pc
  000b6bf2: ldr r1,[r1,#0x0]
  000b6bf4: ldr r1,[r1,#0x0]
  000b6bf6: ldr.w r1,[r1,#0x80]
  000b6bfa: subs r0,r0,r1
  000b6bfc: clz r0,r0
  000b6c00: lsrs r0,r0,#0x5
  000b6c02: bx lr
```
