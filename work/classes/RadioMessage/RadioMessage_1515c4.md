# RadioMessage::RadioMessage
elf 0x1515c4 body 102
Sig: undefined __thiscall RadioMessage(RadioMessage * this, int param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* RadioMessage::RadioMessage(int, int, int, int, int) */

RadioMessage * __thiscall
RadioMessage::RadioMessage
          (RadioMessage *this,int param_1,int param_2,int param_3,int param_4,int param_5)

{
  uint uVar1;
  void *pvVar2;
  int iVar3;
  
  iVar3 = 0;
  uVar1 = (uint)((ulonglong)(uint)param_5 * 4);
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(int *)(this + 8) = param_1;
  *(int *)(this + 0xc) = param_2;
  *(int *)(this + 0x10) = param_3;
  *(int *)(this + 0x14) = param_4;
  if ((int)((ulonglong)(uint)param_5 * 4 >> 0x20) != 0) {
    uVar1 = 0xffffffff;
  }
  pvVar2 = operator_new__(uVar1);
  *(void **)(this + 0x1c) = pvVar2;
  for (; iVar3 < param_5; iVar3 = iVar3 + 1) {
    *(int *)((int)pvVar2 + iVar3 * 4) = param_4 + iVar3;
  }
  *(int *)(this + 0x18) = param_5;
  *(undefined2 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  return this;
}

```

## target disasm
```
  001615c4: push {r4,r5,r6,r7,lr}
  001615c6: add r7,sp,#0xc
  001615c8: push.w r8
  001615cc: mov r8,r0
  001615ce: ldrd r6,r5,[r7,#0x8]
  001615d2: movs r0,#0x4
  001615d4: movs r4,#0x0
  001615d6: umull r0,r12,r5,r0
  001615da: add.w lr,r8,#0x8
  001615de: str.w r4,[r8,#0x1c]
  001615e2: strd r4,r4,[r8,#0x0]
  001615e6: stm.w lr!,{r1,r2,r3,r6}
  001615ea: cmp.w r12,#0x0
  001615ee: it ne
  001615f0: mov.ne.w r12,#0x1
  001615f4: cmp.w r12,#0x0
  001615f8: it ne
  001615fa: mov.ne.w r0,#0xffffffff
  001615fe: blx 0x0006ec08
  00161602: str.w r0,[r8,#0x1c]
  00161606: b 0x00161610
  00161608: adds r1,r6,r4
  0016160a: str.w r1,[r0,r4,lsl #0x2]
  0016160e: adds r4,#0x1
  00161610: cmp r4,r5
  00161612: blt 0x00161608
  00161614: movs r0,#0x0
  00161616: str.w r5,[r8,#0x18]
  0016161a: strh.w r0,[r8,#0x20]
  0016161e: str.w r0,[r8,#0x24]
  00161622: mov r0,r8
  00161624: pop.w r8
  00161628: pop {r4,r5,r6,r7,pc}
```
