# Route::getWaypoint
elf 0x11be34 body 158
Sig: undefined __thiscall getWaypoint(Route * this, int param_1)

## decompile
```c

/* Route::getWaypoint(int) */

void __thiscall Route::getWaypoint(Route *this,int param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  float local_28;
  float local_24;
  float local_20;
  int local_1c;
  
  piVar3 = *(int **)(DAT_0012bed4 + 0x12be48);
  local_1c = *piVar3;
  if (((param_1 < **(int **)(this + 0xc)) &&
      (iVar2 = *(int *)((*(int **)(this + 0xc))[1] + param_1 * 4), iVar2 != 0)) &&
     (piVar1 = *(int **)(*(int *)(*(int *)(this + 0x10) + 4) + param_1 * 4), piVar1 != (int *)0x0))
  {
    (**(code **)(*piVar1 + 0x28))(&local_28);
    *(int *)(iVar2 + 0x124) = (int)local_28;
    (**(code **)(**(int **)(*(int *)(*(int *)(this + 0x10) + 4) + param_1 * 4) + 0x28))(&local_28);
    *(int *)(iVar2 + 0x128) = (int)local_24;
    (**(code **)(**(int **)(*(int *)(*(int *)(this + 0x10) + 4) + param_1 * 4) + 0x28))(&local_28);
    *(int *)(iVar2 + 300) = (int)local_20;
  }
  iVar2 = *piVar3 - local_1c;
  if (iVar2 == 0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

```

## target disasm
```
  0012be34: push {r4,r5,r6,r7,lr}
  0012be36: add r7,sp,#0xc
  0012be38: push.w r8
  0012be3c: sub sp,#0x10
  0012be3e: mov r6,r0
  0012be40: ldr r0,[0x0012bed4]
  0012be42: mov r5,r1
  0012be44: add r0,pc
  0012be46: ldr.w r8,[r0,#0x0]
  0012be4a: ldr.w r0,[r8,#0x0]
  0012be4e: str r0,[sp,#0xc]
  0012be50: ldr r0,[r6,#0xc]
  0012be52: ldr r1,[r0,#0x0]
  0012be54: cmp r1,r5
  0012be56: ble 0x0012beb8
  0012be58: ldr r0,[r0,#0x4]
  0012be5a: ldr.w r4,[r0,r5,lsl #0x2]
  0012be5e: cbz r4,0x0012beb8
  0012be60: ldr r0,[r6,#0x10]
  0012be62: ldr r0,[r0,#0x4]
  0012be64: ldr.w r1,[r0,r5,lsl #0x2]
  0012be68: cbz r1,0x0012beba
  0012be6a: ldr r0,[r1,#0x0]
  0012be6c: ldr r2,[r0,#0x28]
  0012be6e: mov r0,sp
  0012be70: blx r2
  0012be72: vldr.32 s0,[sp]
  0012be76: vcvt.s32.f32 s0,s0
  0012be7a: vstr.32 s0,[r4,#0x124]
  0012be7e: ldr r0,[r6,#0x10]
  0012be80: ldr r0,[r0,#0x4]
  0012be82: ldr.w r1,[r0,r5,lsl #0x2]
  0012be86: ldr r0,[r1,#0x0]
  0012be88: ldr r2,[r0,#0x28]
  0012be8a: mov r0,sp
  0012be8c: blx r2
  0012be8e: vldr.32 s0,[sp,#0x4]
  0012be92: vcvt.s32.f32 s0,s0
  0012be96: vstr.32 s0,[r4,#0x128]
  0012be9a: ldr r0,[r6,#0x10]
  0012be9c: ldr r0,[r0,#0x4]
  0012be9e: ldr.w r1,[r0,r5,lsl #0x2]
  0012bea2: ldr r0,[r1,#0x0]
  0012bea4: ldr r2,[r0,#0x28]
  0012bea6: mov r0,sp
  0012bea8: blx r2
  0012beaa: vldr.32 s0,[sp,#0x8]
  0012beae: vcvt.s32.f32 s0,s0
  0012beb2: vstr.32 s0,[r4,#0x12c]
  0012beb6: b 0x0012beba
  0012beb8: movs r4,#0x0
  0012beba: ldr r0,[sp,#0xc]
  0012bebc: ldr.w r1,[r8,#0x0]
  0012bec0: subs r0,r1,r0
  0012bec2: itttt eq
  0012bec4: mov.eq r0,r4
  0012bec6: add.eq sp,#0x10
  0012bec8: pop.eq.w r8
  0012becc: pop.eq {r4,r5,r6,r7,pc}
  0012bece: blx 0x0006e824
```
