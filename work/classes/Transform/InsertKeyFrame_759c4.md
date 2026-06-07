# Transform::InsertKeyFrame
elf 0x759c4 body 68
Sig: undefined __thiscall InsertKeyFrame(Transform * this, KeyFrame * param_1, int param_2)

## decompile
```c

/* AbyssEngine::Transform::InsertKeyFrame(AbyssEngine::KeyFrame*, int) */

void __thiscall
AbyssEngine::Transform::InsertKeyFrame(Transform *this,KeyFrame *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  ArrayAdd<AbyssEngine::KeyFrame*>((KeyFrame *)0x0,(Array *)(this + 0x11c));
  iVar2 = *(int *)(this + 0x11c);
  iVar3 = iVar2 + -2;
  while (iVar2 = iVar2 + -1, param_2 < iVar2) {
    iVar1 = iVar3 * 4;
    iVar3 = iVar3 + -1;
    *(undefined4 *)(*(int *)(this + 0x120) + iVar2 * 4) =
         *(undefined4 *)(*(int *)(this + 0x120) + iVar1);
  }
  *(KeyFrame **)(*(int *)(this + 0x120) + param_2 * 4) = param_1;
  return;
}

```

## target disasm
```
  000859c4: push {r4,r5,r6,r7,lr}
  000859c6: add r7,sp,#0xc
  000859c8: push.w r11
  000859cc: mov r5,r1
  000859ce: add.w r1,r0,#0x11c
  000859d2: mov r6,r0
  000859d4: movs r0,#0x0
  000859d6: mov r4,r2
  000859d8: blx 0x0006f844
  000859dc: ldr.w r1,[r6,#0x11c]
  000859e0: subs r0,r1,#0x1
  000859e2: subs r1,#0x2
  000859e4: b 0x000859f6
  000859e6: ldr.w r2,[r6,#0x120]
  000859ea: ldr.w r3,[r2,r1,lsl #0x2]
  000859ee: subs r1,#0x1
  000859f0: str.w r3,[r2,r0,lsl #0x2]
  000859f4: subs r0,#0x1
  000859f6: cmp r0,r4
  000859f8: bgt 0x000859e6
  000859fa: ldr.w r0,[r6,#0x120]
  000859fe: str.w r5,[r0,r4,lsl #0x2]
  00085a02: pop.w r11
  00085a06: pop {r4,r5,r6,r7,pc}
```
