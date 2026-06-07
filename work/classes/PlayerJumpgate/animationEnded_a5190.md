# PlayerJumpgate::animationEnded
elf 0xa5190 body 42
Sig: undefined __thiscall animationEnded(PlayerJumpgate * this)

## decompile
```c

/* PlayerJumpgate::animationEnded() */

bool __thiscall PlayerJumpgate::animationEnded(PlayerJumpgate *this)

{
  int iVar1;
  
  if (this[0x140] != (PlayerJumpgate)0x0) {
    iVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_000b51bc + 0xb51a0));
    return *(char *)(iVar1 + 0xed) == '\0';
  }
  return false;
}

```

## target disasm
```
  000b5190: ldrb.w r1,[r0,#0x140]
  000b5194: cbz r1,0x000b51b6
  000b5196: push {r7,lr}
  000b5198: mov r7,sp
  000b519a: ldr r1,[0x000b51bc]
  000b519c: add r1,pc
  000b519e: ldr r2,[r1,#0x0]
  000b51a0: ldr.w r1,[r0,#0x144]
  000b51a4: ldr r0,[r2,#0x0]
  000b51a6: blx 0x00072088
  000b51aa: ldrb.w r0,[r0,#0xed]
  000b51ae: clz r0,r0
  000b51b2: lsrs r0,r0,#0x5
  000b51b4: pop {r7,pc}
  000b51b6: movs r0,#0x0
  000b51b8: bx lr
```
