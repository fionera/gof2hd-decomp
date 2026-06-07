# PlayerEgo::setAutoPilot
elf 0x9b808 body 94
Sig: undefined __stdcall setAutoPilot(KIPlayer * param_1)

## decompile
```c

/* PlayerEgo::setAutoPilot(KIPlayer*) */

void PlayerEgo::setAutoPilot(KIPlayer *param_1)

{
  KIPlayer KVar1;
  int iVar2;
  int in_r1;
  KIPlayer KVar3;
  
  param_1[0x160] = (KIPlayer)0x0;
  KVar3 = SUB41(in_r1,0);
  *(int *)(param_1 + 0x15c) = in_r1;
  KVar1 = param_1[0x158];
  if (in_r1 != 0) {
    KVar3 = (KIPlayer)0x1;
  }
  param_1[0x158] = KVar3;
  if (in_r1 == 0) {
    *(undefined4 *)(*(int *)(param_1 + 0x14) + 0x2c) = 0;
    if (KVar1 != (KIPlayer)0x0) {
      param_1[0x2a8] = (KIPlayer)0x0;
      *(undefined4 *)(param_1 + 0x2a4) = 0;
      return;
    }
  }
  else {
    if (*(char *)(in_r1 + 0x72) != '\0') {
      param_1[0x160] = (KIPlayer)0x1;
    }
    iVar2 = AbyssEngine::ApplicationManager::GetEngine();
    *(undefined4 *)(iVar2 + 0x360) = 0;
    *(undefined4 *)(param_1 + 0xbc) = 0x3f800000;
  }
  return;
}

```

## target disasm
```
  000ab808: push {r4,r6,r7,lr}
  000ab80a: add r7,sp,#0x8
  000ab80c: mov r4,r0
  000ab80e: movs r0,#0x0
  000ab810: strb.w r0,[r4,#0x160]
  000ab814: mov r3,r1
  000ab816: str.w r1,[r4,#0x15c]
  000ab81a: cmp r1,#0x0
  000ab81c: ldrb.w r2,[r4,#0x158]
  000ab820: it ne
  000ab822: mov.ne r3,#0x1
  000ab824: strb.w r3,[r4,#0x158]
  000ab828: beq 0x000ab852
  000ab82a: ldrb.w r0,[r1,#0x72]
  000ab82e: cbz r0,0x000ab836
  000ab830: movs r0,#0x1
  000ab832: strb.w r0,[r4,#0x160]
  000ab836: ldr r0,[0x000ab868]
  000ab838: add r0,pc
  000ab83a: ldr r0,[r0,#0x0]
  000ab83c: ldr r0,[r0,#0x0]
  000ab83e: blx 0x0007258c
  000ab842: movs r1,#0x0
  000ab844: str.w r1,[r0,#0x360]
  000ab848: mov.w r0,#0x3f800000
  000ab84c: str.w r0,[r4,#0xbc]
  000ab850: pop {r4,r6,r7,pc}
  000ab852: ldr r1,[r4,#0x14]
  000ab854: cmp r2,#0x0
  000ab856: str r0,[r1,#0x2c]
  000ab858: beq 0x000ab850
  000ab85a: movs r0,#0x0
  000ab85c: strb.w r0,[r4,#0x2a8]
  000ab860: str.w r0,[r4,#0x2a4]
  000ab864: pop {r4,r6,r7,pc}
```
