# Layout::drawMask
elf 0xd3140 body 32
Sig: undefined __stdcall drawMask(void)

## decompile
```c

/* Layout::drawMask() */

void Layout::drawMask(void)

{
  drawMask((Layout *)**(undefined4 **)(DAT_000e3164 + 0xe3150),0,0,
           **(int **)(DAT_000e3160 + 0xe314e),(int)**(undefined4 **)(DAT_000e3164 + 0xe3150));
  return;
}

```

## target disasm
```
  000e3140: push {r5,r6,r7,lr}
  000e3142: add r7,sp,#0x8
  000e3144: ldr r0,[0x000e3160]
  000e3146: movs r2,#0x0
  000e3148: ldr r1,[0x000e3164]
  000e314a: add r0,pc
  000e314c: add r1,pc
  000e314e: ldr r0,[r0,#0x0]
  000e3150: ldr r1,[r1,#0x0]
  000e3152: ldr r3,[r0,#0x0]
  000e3154: ldr r0,[r1,#0x0]
  000e3156: movs r1,#0x0
  000e3158: str r0,[sp,#0x0]
  000e315a: blx 0x00074ddc
  000e315e: pop {r2,r3,r7,pc}
```
