# Sparks::translate
elf 0x15e6d4 body 72
Sig: undefined __stdcall translate(Vector * param_1)

## decompile
```c

/* Sparks::translate(AbyssEngine::AEMath::Vector const&) */

void Sparks::translate(Vector *param_1)

{
  int in_r1;
  uint uVar1;
  uint *puVar2;
  float in_s1;
  float extraout_s1;
  
  puVar2 = *(uint **)(DAT_0016e71c + 0x16e6ea);
  for (uVar1 = 0; uVar1 < *(uint *)(param_1 + 0x18); uVar1 = uVar1 + 1) {
    AbyssEngine::PaintCanvas::SpriteSystemAddPosition
              (*puVar2,(ushort)*(undefined4 *)(param_1 + 4),*(float *)(in_r1 + 4),in_s1,
               *(float *)(in_r1 + 8));
    in_s1 = extraout_s1;
  }
  return;
}

```

## target disasm
```
  0016e6d4: push {r4,r5,r6,r7,lr}
  0016e6d6: add r7,sp,#0xc
  0016e6d8: push.w r8
  0016e6dc: sub sp,#0x8
  0016e6de: mov r5,r0
  0016e6e0: ldr r0,[0x0016e71c]
  0016e6e2: mov r4,r1
  0016e6e4: movs r6,#0x0
  0016e6e6: add r0,pc
  0016e6e8: ldr.w r8,[r0,#0x0]
  0016e6ec: b 0x0016e70e
  0016e6ee: ldr r3,[r4,#0x0]
  0016e6f0: uxth r2,r6
  0016e6f2: ldr r1,[r5,#0x4]
  0016e6f4: ldr.w r0,[r8,#0x0]
  0016e6f8: vldr.32 s0,[r4,#0x4]
  0016e6fc: vldr.32 s2,[r4,#0x8]
  0016e700: vstr.32 s0,[sp]
  0016e704: vstr.32 s2,[sp,#0x4]
  0016e708: blx 0x000778fc
  0016e70c: adds r6,#0x1
  0016e70e: ldr r0,[r5,#0x18]
  0016e710: cmp r6,r0
  0016e712: bcc 0x0016e6ee
  0016e714: add sp,#0x8
  0016e716: pop.w r8
  0016e71a: pop {r4,r5,r6,r7,pc}
```
