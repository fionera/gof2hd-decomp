# AbyssEngine::AEMath::MatrixIdentity
elf 0x80694 body 102
Sig: undefined __thiscall MatrixIdentity(AEMath * this, Matrix * param_1)

## decompile
```c

/* AbyssEngine::AEMath::MatrixIdentity(AbyssEngine::AEMath::Matrix&) */

void __thiscall AbyssEngine::AEMath::MatrixIdentity(AEMath *this,Matrix *param_1)

{
  int *piVar1;
  undefined4 local_28;
  undefined4 uStack_24;
  undefined4 local_20;
  int local_1c;
  
  piVar1 = *(int **)(DAT_000906fc + 0x906ac);
  local_1c = *piVar1;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(param_1 + 0x18) = 0;
  *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(param_1 + 0x24) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)param_1 = 0x3f800000;
  *(undefined4 *)(param_1 + 0x14) = 0x3f800000;
  *(undefined4 *)(param_1 + 0x28) = 0x3f800000;
  *(undefined4 *)(param_1 + 0x2c) = 0;
  local_28 = 0x3f800000;
  uStack_24 = 0x3f800000;
  local_20 = 0x3f800000;
  Vector::operator=((Vector *)(param_1 + 0x30),(Vector *)&local_28);
  __aeabi_memcpy(this,param_1,0x3c);
  if (*piVar1 != local_1c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```
## target disasm
```
  00090694: push {r4,r5,r6,r7,lr}
  00090696: add r7,sp,#0xc
  00090698: push.w r11
  0009069c: sub sp,#0x10
  0009069e: mov r5,r0
  000906a0: ldr r0,[0x000906fc]
  000906a2: vmov.i32 q8,#0x0
  000906a6: mov r4,r1
  000906a8: add r0,pc
  000906aa: ldr r6,[r0,#0x0]
  000906ac: ldr r0,[r6,#0x0]
  000906ae: str r0,[sp,#0xc]
  000906b0: adds r0,r1,#0x4
  000906b2: vst1.32 {d16,d17},[r0]
  000906b6: add.w r0,r1,#0x18
  000906ba: vst1.32 {d16,d17},[r0]
  000906be: mov.w r0,#0x3f800000
  000906c2: str r0,[r1,#0x0]
  000906c4: str r0,[r1,#0x14]
  000906c6: movs r1,#0x0
  000906c8: strd r0,r1,[r4,#0x28]
  000906cc: mov r1,sp
  000906ce: strd r0,r0,[sp,#0x0]
  000906d2: str r0,[sp,#0x8]
  000906d4: add.w r0,r4,#0x30
  000906d8: blx 0x0006eb3c
  000906dc: mov r0,r5
  000906de: mov r1,r4
  000906e0: movs r2,#0x3c
  000906e2: blx 0x0006f1e4
  000906e6: ldr r0,[sp,#0xc]
  000906e8: ldr r1,[r6,#0x0]
  000906ea: subs r0,r1,r0
  000906ec: ittt eq
  000906ee: add.eq sp,#0x10
  000906f0: pop.eq.w r11
  000906f4: pop.eq {r4,r5,r6,r7,pc}
  000906f6: blx 0x0006e824
```
