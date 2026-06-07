# Layout::getFooterTransitionWidth
elf 0xd3c08 body 46
Sig: undefined __stdcall getFooterTransitionWidth(void)

## decompile
```c

/* Layout::getFooterTransitionWidth() */

int Layout::getFooterTransitionWidth(void)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  
  puVar3 = *(uint **)(DAT_000e3c38 + 0xe3c1c);
  iVar1 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar3);
  iVar2 = AbyssEngine::PaintCanvas::GetImage2DWidth(*puVar3);
  return iVar2 + iVar1;
}

```

## target disasm
```
  000e3c08: push {r4,r5,r6,r7,lr}
  000e3c0a: add r7,sp,#0xc
  000e3c0c: push.w r11
  000e3c10: mov r4,r0
  000e3c12: ldr r0,[0x000e3c38]
  000e3c14: ldr.w r1,[r4,#0x33c]
  000e3c18: add r0,pc
  000e3c1a: ldr r6,[r0,#0x0]
  000e3c1c: ldr r0,[r6,#0x0]
  000e3c1e: blx 0x00072d84
  000e3c22: mov r5,r0
  000e3c24: ldr r0,[r6,#0x0]
  000e3c26: ldr.w r1,[r4,#0x334]
  000e3c2a: blx 0x00072d84
  000e3c2e: add r0,r5
  000e3c30: pop.w r11
  000e3c34: pop {r4,r5,r6,r7,pc}
```
