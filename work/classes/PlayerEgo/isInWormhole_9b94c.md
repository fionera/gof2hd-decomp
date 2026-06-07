# PlayerEgo::isInWormhole
elf 0x9b94c body 30
Sig: undefined __stdcall isInWormhole(void)

## decompile
```c

/* PlayerEgo::isInWormhole() */

bool PlayerEgo::isInWormhole(void)

{
  int in_r0;
  int iVar1;
  
  iVar1 = getHitpoints();
  if (0 < iVar1) {
    return *(char *)(in_r0 + 0x25) != '\0';
  }
  return false;
}

```

## target disasm
```
  000ab94c: push {r4,r6,r7,lr}
  000ab94e: add r7,sp,#0x8
  000ab950: mov r4,r0
  000ab952: blx 0x000724e4
  000ab956: cmp r0,#0x1
  000ab958: blt 0x000ab966
  000ab95a: ldrb.w r0,[r4,#0x25]
  000ab95e: cmp r0,#0x0
  000ab960: it ne
  000ab962: mov.ne r0,#0x1
  000ab964: pop {r4,r6,r7,pc}
  000ab966: movs r0,#0x0
  000ab968: pop {r4,r6,r7,pc}
```
