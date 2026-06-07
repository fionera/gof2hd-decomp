# Engine::LightSetLightPosition
elf 0x8595c body 126
Sig: undefined __thiscall LightSetLightPosition(Engine * this, float param_1, float param_2, float param_3, uint param_4)

## decompile
```c

/* AbyssEngine::Engine::LightSetLightPosition(float, float, float, unsigned int) */

void __thiscall
AbyssEngine::Engine::LightSetLightPosition
          (Engine *this,float param_1,float param_2,float param_3,uint param_4)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  float extraout_s0;
  float extraout_s1;
  float extraout_s2;
  int in_stack_00000000;
  uint local_28 [3];
  int local_1c;
  
  piVar2 = *(int **)(DAT_000959dc + 0x95970);
  local_1c = *piVar2;
  uVar3 = in_stack_00000000 - 0x4000;
  if (uVar3 < 8) {
    iVar1 = in_stack_00000000 + -0x3fff;
    if (in_stack_00000000 + -0x3fff < *(int *)(this + 0x32c)) {
      iVar1 = *(int *)(this + 0x32c);
    }
    *(int *)(this + 0x32c) = iVar1;
    local_28[0] = param_4;
    AEMath::Vector::operator=((Vector *)(this + uVar3 * 0xc + 0x468),(Vector *)local_28);
    *(undefined4 *)(this + uVar3 * 4 + 0x378) = 0x3f800000;
    param_1 = extraout_s0;
    param_2 = extraout_s1;
    param_3 = extraout_s2;
  }
  if (*piVar2 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_1,param_2,param_3);
  }
  return;
}

```

## target disasm
```
  0009595c: push {r4,r5,r6,r7,lr}
  0009595e: add r7,sp,#0xc
  00095960: push.w r11
  00095964: sub sp,#0x10
  00095966: ldr r5,[0x000959dc]
  00095968: mov r4,r0
  0009596a: ldr r0,[r7,#0x8]
  0009596c: add r5,pc
  0009596e: ldr r5,[r5,#0x0]
  00095970: ldr r6,[r5,#0x0]
  00095972: str r6,[sp,#0xc]
  00095974: sub.w r6,r0,#0x4000
  00095978: cmp r6,#0x7
  0009597a: bhi 0x000959c6
  0009597c: vmov s2,r2
  00095980: ldr.w r2,[r4,#0x32c]
  00095984: vmov s4,r1
  00095988: movw r1,#0x3fff
  0009598c: subs r0,r0,r1
  0009598e: vmov s0,r3
  00095992: cmp r2,r0
  00095994: it gt
  00095996: mov.gt r0,r2
  00095998: str.w r0,[r4,#0x32c]
  0009599c: add.w r0,r6,r6, lsl #0x1
  000959a0: mov r1,sp
  000959a2: vstr.32 s2,[sp,#0x4]
  000959a6: add.w r0,r4,r0, lsl #0x2
  000959aa: vstr.32 s4,[sp]
  000959ae: add.w r0,r0,#0x468
  000959b2: vstr.32 s0,[sp,#0x8]
  000959b6: blx 0x0006eb3c
  000959ba: add.w r0,r4,r6, lsl #0x2
  000959be: mov.w r1,#0x3f800000
  000959c2: str.w r1,[r0,#0x378]
  000959c6: ldr r0,[sp,#0xc]
  000959c8: ldr r1,[r5,#0x0]
  000959ca: subs r0,r1,r0
  000959cc: ittt eq
  000959ce: add.eq sp,#0x10
  000959d0: pop.eq.w r11
  000959d4: pop.eq {r4,r5,r6,r7,pc}
  000959d6: blx 0x0006e824
```
