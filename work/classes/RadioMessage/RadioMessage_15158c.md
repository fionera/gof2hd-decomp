# RadioMessage::RadioMessage
elf 0x15158c body 48
Sig: undefined __thiscall RadioMessage(RadioMessage * this, int param_1, int param_2, int param_3, int param_4)

## decompile
```c

/* RadioMessage::RadioMessage(int, int, int, int) */

RadioMessage * __thiscall
RadioMessage::RadioMessage(RadioMessage *this,int param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(int *)(this + 8) = param_1;
  *(int *)(this + 0xc) = param_2;
  *(int *)(this + 0x10) = param_3;
  *(int *)(this + 0x14) = param_4;
  piVar1 = operator_new__(4);
  *piVar1 = param_4;
  *(undefined4 *)(this + 0x18) = 1;
  *(int **)(this + 0x1c) = piVar1;
  *(undefined2 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  return this;
}

```

## target disasm
```
  0016158c: push {r4,r5,r6,r7,lr}
  0016158e: add r7,sp,#0xc
  00161590: push.w r11
  00161594: ldr r5,[r7,#0x8]
  00161596: movs r6,#0x0
  00161598: mov r4,r0
  0016159a: str r6,[r0,#0x1c]
  0016159c: strd r6,r6,[r0],#0x8
  001615a0: stmia r0!,{r1,r2,r3,r5}
  001615a2: movs r0,#0x4
  001615a4: blx 0x0006ec08
  001615a8: movs r1,#0x1
  001615aa: str r5,[r0,#0x0]
  001615ac: strd r1,r0,[r4,#0x18]
  001615b0: mov r0,r4
  001615b2: strh r6,[r4,#0x20]
  001615b4: str r6,[r4,#0x24]
  001615b6: pop.w r11
  001615ba: pop {r4,r5,r6,r7,pc}
```
