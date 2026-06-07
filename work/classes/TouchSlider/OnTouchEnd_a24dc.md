# TouchSlider::OnTouchEnd
elf 0xa24dc body 144
Sig: undefined __stdcall OnTouchEnd(int param_1, int param_2)

## decompile
```c

/* TouchSlider::OnTouchEnd(int, int) */

undefined4 TouchSlider::OnTouchEnd(int param_1,int param_2)

{
  undefined4 uVar1;
  float fVar2;
  int iVar3;
  int iVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  float fVar7;
  
  if (*(char *)(param_1 + 0x35) == '\0') {
    if (*(char *)(param_1 + 0x34) == '\0') {
      uVar1 = 0;
    }
    else {
      iVar4 = *(int *)(param_1 + 0x28);
      if (0 < iVar4) {
        fVar6 = (float)VectorSignedToFloat(iVar4 + 1,(byte)(in_fpscr >> 0x16) & 3);
        fVar2 = (float)getValue((TouchSlider *)param_1);
        iVar3 = 0;
        for (fVar7 = DAT_000b256c;
            (fVar5 = fVar2, iVar3 < iVar4 + 2 &&
            (fVar5 = fVar7, (1.0 / fVar6) * 0.5 + fVar7 < fVar2)); fVar7 = 1.0 / fVar6 + fVar7) {
          iVar3 = iVar3 + 1;
        }
        setValue((TouchSlider *)param_1,fVar5);
      }
      uVar1 = 1;
    }
    *(undefined1 *)(param_1 + 0x34) = 0;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

```

## target disasm
```
  000b24dc: push {r4,r5,r6,r7,lr}
  000b24de: add r7,sp,#0xc
  000b24e0: push.w r11
  000b24e4: vpush {d8,d9}
  000b24e8: mov r4,r0
  000b24ea: ldrb.w r0,[r0,#0x35]
  000b24ee: cbz r0,0x000b24f4
  000b24f0: movs r0,#0x0
  000b24f2: b 0x000b2562
  000b24f4: ldrb.w r0,[r4,#0x34]
  000b24f8: movs r5,#0x0
  000b24fa: cbz r0,0x000b254a
  000b24fc: ldr r6,[r4,#0x28]
  000b24fe: cmp r6,#0x1
  000b2500: blt 0x000b255c
  000b2502: adds r0,r6,#0x1
  000b2504: vmov.f32 s0,0x3f800000
  000b2508: vmov s2,r0
  000b250c: mov r0,r4
  000b250e: vcvt.f32.s32 s2,s2
  000b2512: vdiv.f32 s16,s0,s2
  000b2516: vmov.f32 s0,0x3f000000
  000b251a: vmul.f32 s18,s16,s0
  000b251e: blx 0x00072dd8
  000b2522: vmov s0,r0
  000b2526: vldr.32 s2,[pc,#0x44]
  000b252a: adds r1,r6,#0x2
  000b252c: movs r0,#0x0
  000b252e: b 0x000b2544
  000b2530: vadd.f32 s4,s18,s2
  000b2534: vcmpe.f32 s0,s4
  000b2538: vmrs apsr,fpscr
  000b253c: bls 0x000b254e
  000b253e: vadd.f32 s2,s16,s2
  000b2542: adds r0,#0x1
  000b2544: cmp r0,r1
  000b2546: blt 0x000b2530
  000b2548: b 0x000b2552
  000b254a: movs r0,#0x0
  000b254c: b 0x000b255e
  000b254e: vmov.f32 s0,s2
  000b2552: vmov r1,s0
  000b2556: mov r0,r4
  000b2558: blx 0x00072de4
  000b255c: movs r0,#0x1
  000b255e: strb.w r5,[r4,#0x34]
  000b2562: vpop {d8,d9}
  000b2566: pop.w r11
  000b256a: pop {r4,r5,r6,r7,pc}
```
