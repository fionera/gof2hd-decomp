# PlayerJumpgate::timeToJump
elf 0xa5164 body 40
Sig: undefined __stdcall timeToJump(void)

## decompile
```c

/* PlayerJumpgate::timeToJump() */

bool PlayerJumpgate::timeToJump(void)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  
  iVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(**(uint **)(DAT_000b518c + 0xb516e));
  iVar2 = *(int *)(iVar1 + 0x114);
  bVar3 = 1000 < *(uint *)(iVar1 + 0x110);
  return (int)(-(uint)bVar3 - iVar2) < 0 != (SBORROW4(0,iVar2) != SBORROW4(-iVar2,(uint)bVar3));
}

```

## target disasm
```
  000b5164: push {r7,lr}
  000b5166: mov r7,sp
  000b5168: ldr r1,[0x000b518c]
  000b516a: add r1,pc
  000b516c: ldr r2,[r1,#0x0]
  000b516e: ldr.w r1,[r0,#0x144]
  000b5172: ldr r0,[r2,#0x0]
  000b5174: blx 0x00072088
  000b5178: ldrd r1,r2,[r0,#0x110]
  000b517c: movs r0,#0x0
  000b517e: rsbs.w r1,r1,#0x3e8
  000b5182: sbcs.w r1,r0,r2
  000b5186: it lt
  000b5188: mov.lt r0,#0x1
  000b518a: pop {r7,pc}
```
