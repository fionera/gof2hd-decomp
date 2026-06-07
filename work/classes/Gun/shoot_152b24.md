# Gun::shoot
elf 0x152b24 body 88
Sig: undefined __stdcall shoot(Matrix param_1, int param_2, bool param_3)

## decompile
```c

/* Gun::shoot(AbyssEngine::AEMath::Matrix, int, bool) */

void Gun::shoot(void)

{
  shootAt();
  return;
}

```

## target disasm
```
  00162b24: push {r4,r5,r6,r7,lr}
  00162b26: add r7,sp,#0xc
  00162b28: push {r8,r9,r10,r11}
  00162b2c: sub sp,#0x3c
  00162b2e: add.w lr,r7,#0x20
  00162b32: ldr r5,[r7,#0x34]
  00162b34: movs r4,#0x0
  00162b36: ldr.w r11,[r7,#0x18]
  00162b3a: ldm.w lr,{r8,r9,lr}
  00162b3e: ldrd r12,r6,[r7,#0x2c]
  00162b42: strd r4,r4,[sp,#0x34]
  00162b46: str r5,[sp,#0x2c]
  00162b48: str.w r12,[sp,#0x24]
  00162b4c: ldr.w r12,[r7,#0x10]
  00162b50: ldr r5,[r7,#0xc]
  00162b52: ldr r4,[r7,#0x8]
  00162b54: str r6,[sp,#0x28]
  00162b56: ldr.w r10,[r7,#0x1c]
  00162b5a: ldr r6,[r7,#0x14]
  00162b5c: str.w lr,[sp,#0x20]
  00162b60: str.w r9,[sp,#0x1c]
  00162b64: strd r10,r8,[sp,#0x14]
  00162b68: strd r6,r11,[sp,#0xc]
  00162b6c: stm sp,{r4,r5,r12}
  00162b70: blx 0x00072fb8
  00162b74: add sp,#0x3c
  00162b76: pop.w {r8,r9,r10,r11}
  00162b7a: pop {r4,r5,r6,r7,pc}
```
