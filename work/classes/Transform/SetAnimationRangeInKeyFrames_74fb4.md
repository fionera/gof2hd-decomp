# Transform::SetAnimationRangeInKeyFrames
elf 0x74fb4 body 80
Sig: undefined __thiscall SetAnimationRangeInKeyFrames(Transform * this, int param_1, int param_2)

## decompile
```c

/* AbyssEngine::Transform::SetAnimationRangeInKeyFrames(int, int) */

void __thiscall
AbyssEngine::Transform::SetAnimationRangeInKeyFrames(Transform *this,int param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  if ((param_1 < 0) || (*(int *)(this + 0x11c) <= param_1)) {
    uVar2 = 0;
    uVar3 = 0;
  }
  else {
    param_1 = *(int *)(*(int *)(this + 0x120) + param_1 * 4);
    uVar2 = *(undefined4 *)(param_1 + 0x50);
    uVar3 = *(undefined4 *)(param_1 + 0x54);
  }
  if (-1 < param_2) {
    param_1 = *(int *)(this + 0x11c);
  }
  if (-1 < param_2 && param_2 <= param_1) {
    uVar1 = *(undefined4 *)(*(int *)(*(int *)(this + 0x120) + param_2 * 4) + 0x54);
  }
  else {
    uVar1 = 0;
  }
  SetAnimationRangeInTime(CONCAT44(uVar1,this),CONCAT44(uVar3,uVar2));
  return;
}

```

## target disasm
```
  00084fb4: mov r12,r2
  00084fb6: cmp r1,#0x0
  00084fb8: blt 0x00084fd0
  00084fba: ldr.w r2,[r0,#0x11c]
  00084fbe: cmp r2,r1
  00084fc0: ble 0x00084fd0
  00084fc2: ldr.w r2,[r0,#0x120]
  00084fc6: ldr.w r1,[r2,r1,lsl #0x2]
  00084fca: ldrd r2,r3,[r1,#0x50]
  00084fce: b 0x00084fd4
  00084fd0: movs r2,#0x0
  00084fd2: movs r3,#0x0
  00084fd4: cmp.w r12,#0x0
  00084fd8: itt ge
  00084fda: ldr.ge.w r1,[r0,#0x11c]
  00084fde: cmp.ge r1,r12
  00084fe0: bge 0x00084fea
  00084fe2: mov.w r12,#0x0
  00084fe6: movs r1,#0x0
  00084fe8: b 0x00084ff6
  00084fea: ldr.w r1,[r0,#0x120]
  00084fee: ldr.w r1,[r1,r12,lsl #0x2]
  00084ff2: ldrd r12,r1,[r1,#0x50]
  00084ff6: push {r5,r6,r7,lr}
  00084ff8: add r7,sp,#0x8
  00084ffa: strd r12,r1,[sp,#0x0]
  00084ffe: blx 0x0006ebc0
  00085002: pop {r2,r3,r7,pc}
```
