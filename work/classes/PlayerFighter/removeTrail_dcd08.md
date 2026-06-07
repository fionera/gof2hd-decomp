# PlayerFighter::removeTrail
elf 0xdcd08 body 28
Sig: undefined __thiscall removeTrail(PlayerFighter * this)

## decompile
```c

/* PlayerFighter::removeTrail() */

void __thiscall PlayerFighter::removeTrail(PlayerFighter *this)

{
  void *pvVar1;
  
  if (*(Trail **)(this + 0x154) != (Trail *)0x0) {
    pvVar1 = (void *)Trail::~Trail(*(Trail **)(this + 0x154));
    operator_delete(pvVar1);
  }
  *(undefined4 *)(this + 0x154) = 0;
  return;
}

```

## target disasm
```
  000ecd08: push {r4,r6,r7,lr}
  000ecd0a: add r7,sp,#0x8
  000ecd0c: mov r4,r0
  000ecd0e: ldr.w r0,[r0,#0x154]
  000ecd12: cbz r0,0x000ecd1c
  000ecd14: blx 0x000757c0
  000ecd18: blx 0x0006eb48
  000ecd1c: movs r0,#0x0
  000ecd1e: str.w r0,[r4,#0x154]
  000ecd22: pop {r4,r6,r7,pc}
```
