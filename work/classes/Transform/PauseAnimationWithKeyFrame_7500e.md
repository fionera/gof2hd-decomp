# Transform::PauseAnimationWithKeyFrame
elf 0x7500e body 52
Sig: undefined __thiscall PauseAnimationWithKeyFrame(Transform * this, int param_1)

## decompile
```c

/* AbyssEngine::Transform::PauseAnimationWithKeyFrame(int) */

void __thiscall AbyssEngine::Transform::PauseAnimationWithKeyFrame(Transform *this,int param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  if ((-1 < param_1) && (param_1 < *(int *)(this + 0x11c))) {
    iVar1 = *(int *)(*(int *)(this + 0x120) + param_1 * 4);
    uVar2 = *(undefined4 *)(iVar1 + 0x50);
    uVar3 = *(undefined4 *)(iVar1 + 0x54);
    *(undefined4 *)(this + 0x110) = uVar2;
    *(undefined4 *)(this + 0x114) = uVar3;
    InternUpdate(CONCAT44(param_1,this),SUB41(uVar2,0));
  }
  this[0xed] = (Transform)0x0;
  return;
}

```

## target disasm
```
  0008500e: push {r2,r3,r4,r6,r7,lr}
  00085010: add r7,sp,#0x10
  00085012: mov r4,r0
  00085014: cmp r1,#0x0
  00085016: blt 0x0008503a
  00085018: ldr.w r0,[r4,#0x11c]
  0008501c: cmp r0,r1
  0008501e: ble 0x0008503a
  00085020: ldr.w r0,[r4,#0x120]
  00085024: ldr.w r0,[r0,r1,lsl #0x2]
  00085028: ldrd r2,r3,[r0,#0x50]
  0008502c: movs r0,#0x0
  0008502e: strd r2,r3,[r4,#0x110]
  00085032: str r0,[sp,#0x0]
  00085034: mov r0,r4
  00085036: blx 0x0006f7e4
  0008503a: movs r0,#0x0
  0008503c: strb.w r0,[r4,#0xed]
  00085040: pop {r2,r3,r4,r6,r7,pc}
```
