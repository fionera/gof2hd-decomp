# Layout::drawBG
elf 0xd3544 body 38
Sig: undefined __stdcall drawBG(void)

## decompile
```c

/* Layout::drawBG() */

void Layout::drawBG(void)

{
  Layout *in_r0;
  
  drawBGPattern(in_r0,*(uint *)(in_r0 + 0x324),0,0,**(int **)(DAT_000e356c + 0xe3550),
                **(int **)(DAT_000e3570 + 0xe3552));
  return;
}

```

## target disasm
```
  000e3544: push {r5,r6,r7,lr}
  000e3546: add r7,sp,#0x8
  000e3548: ldr r1,[0x000e356c]
  000e354a: ldr r2,[0x000e3570]
  000e354c: add r1,pc
  000e354e: add r2,pc
  000e3550: ldr r3,[r1,#0x0]
  000e3552: ldr r2,[r2,#0x0]
  000e3554: ldr.w r1,[r0,#0x324]
  000e3558: ldr r3,[r3,#0x0]
  000e355a: ldr r2,[r2,#0x0]
  000e355c: strd r3,r2,[sp,#0x0]
  000e3560: movs r2,#0x0
  000e3562: movs r3,#0x0
  000e3564: blx 0x00074e00
  000e3568: pop {r2,r3,r7,pc}
```
