# AEGeometry::translate
elf 0xa492c body 136
Sig: undefined __stdcall translate(float param_1, float param_2, float param_3)

## decompile
```c

/* AEGeometry::translate(float, float, float) */

void AEGeometry::translate(float param_1,float param_2,float param_3)

{
  int in_r0;
  undefined4 uVar1;
  float in_r2;
  float in_r3;
  int *piVar2;
  float extraout_s1;
  Matrix aMStack_9c [60];
  undefined1 auStack_60 [28];
  float local_44;
  float local_34;
  int local_24;
  
  piVar2 = *(int **)(DAT_000b49b4 + 0xb4942);
  local_24 = *piVar2;
  uVar1 = AbyssEngine::PaintCanvas::TransformGetLocal(*(uint *)(in_r0 + 0x2c));
  __aeabi_memcpy(auStack_60,uVar1,0x3c);
  AbyssEngine::AEMath::MatrixSetTranslation
            (aMStack_9c,local_34 + in_r3,extraout_s1,local_44 + in_r2);
  AbyssEngine::PaintCanvas::TransformSetLocal(*(uint *)(in_r0 + 0x2c),*(Matrix **)(in_r0 + 0xc));
  if (*piVar2 != local_24) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b492c: push {r4,r5,r6,r7,lr}
  000b492e: add r7,sp,#0xc
  000b4930: push {r8,r9,r10}
  000b4934: sub sp,#0x80
  000b4936: mov r4,r0
  000b4938: ldr r0,[0x000b49b4]
  000b493a: mov r6,r1
  000b493c: mov r8,r3
  000b493e: add r0,pc
  000b4940: mov r10,r2
  000b4942: ldr r5,[r0,#0x0]
  000b4944: ldr r0,[r5,#0x0]
  000b4946: str r0,[sp,#0x7c]
  000b4948: ldr r1,[r4,#0xc]
  000b494a: ldr r0,[r4,#0x2c]
  000b494c: blx 0x000720c4
  000b4950: add.w r9,sp,#0x40
  000b4954: mov r1,r0
  000b4956: movs r2,#0x3c
  000b4958: mov r0,r9
  000b495a: blx 0x0006f1e4
  000b495e: vmov s0,r6
  000b4962: vldr.32 s2,[sp,#0x4c]
  000b4966: vldr.32 s4,[sp,#0x5c]
  000b496a: vmov s8,r10
  000b496e: vadd.f32 s0,s2,s0
  000b4972: vldr.32 s6,[sp,#0x6c]
  000b4976: vadd.f32 s2,s4,s8
  000b497a: add r0,sp,#0x4
  000b497c: mov r1,r9
  000b497e: vmov r2,s0
  000b4982: vmov r3,s2
  000b4986: vmov s0,r8
  000b498a: vadd.f32 s0,s6,s0
  000b498e: vstr.32 s0,[sp]
  000b4992: blx 0x0006f820
  000b4996: ldr r1,[r4,#0xc]
  000b4998: mov r2,r9
  000b499a: ldr r0,[r4,#0x2c]
  000b499c: blx 0x000721c0
  000b49a0: ldr r0,[sp,#0x7c]
  000b49a2: ldr r1,[r5,#0x0]
  000b49a4: subs r0,r1,r0
  000b49a6: ittt eq
  000b49a8: add.eq sp,#0x80
  000b49aa: pop.eq.w {r8,r9,r10}
  000b49ae: pop.eq {r4,r5,r6,r7,pc}
  000b49b0: blx 0x0006e824
```
