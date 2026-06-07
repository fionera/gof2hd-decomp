# TouchButton::OnTouchBegin
elf 0x1699d8 body 76
Sig: undefined __stdcall OnTouchBegin(int param_1, int param_2)

## decompile
```c

/* TouchButton::OnTouchBegin(int, int) */

bool TouchButton::OnTouchBegin(int param_1,int param_2)

{
  int iVar1;
  int in_r2;
  bool bVar2;
  float extraout_s0;
  
  if ((*(char *)(param_1 + 0xb2) == '\0') || (*(char *)(param_1 + 0xb3) != '\0')) {
    bVar2 = false;
  }
  else {
    iVar1 = touchedInside((TouchButton *)param_1,param_2,in_r2);
    *(char *)(param_1 + 0xb0) = (char)iVar1;
    bVar2 = false;
    if (iVar1 != 0) {
      FModSound::play(**(int **)(DAT_00179a24 + 0x179a0e),(Vector *)0x7c,(Vector *)0x0,extraout_s0);
      bVar2 = *(char *)(param_1 + 0xb0) != '\0';
    }
  }
  return bVar2;
}

```

## target disasm
```
  001799d8: push {r2,r3,r4,r6,r7,lr}
  001799da: add r7,sp,#0x10
  001799dc: mov r4,r0
  001799de: ldrb.w r0,[r0,#0xb2]
  001799e2: cbz r0,0x001799ea
  001799e4: ldrb.w r0,[r4,#0xb3]
  001799e8: cbz r0,0x001799f0
  001799ea: movs r0,#0x0
  001799ec: add sp,#0x8
  001799ee: pop {r4,r6,r7,pc}
  001799f0: mov r0,r4
  001799f2: blx 0x00077c8c
  001799f6: mov r1,r0
  001799f8: strb.w r0,[r4,#0xb0]
  001799fc: movs r0,#0x0
  001799fe: cmp r1,#0x0
  00179a00: beq 0x001799ec
  00179a02: ldr r1,[0x00179a24]
  00179a04: movs r2,#0x0
  00179a06: str r0,[sp,#0x0]
  00179a08: movs r3,#0x0
  00179a0a: add r1,pc
  00179a0c: ldr r1,[r1,#0x0]
  00179a0e: ldr r1,[r1,#0x0]
  00179a10: mov r0,r1
  00179a12: movs r1,#0x7c
  00179a14: blx 0x00071548
  00179a18: ldrb.w r0,[r4,#0xb0]
  00179a1c: cmp r0,#0x0
  00179a1e: it ne
  00179a20: mov.ne r0,#0x1
  00179a22: b 0x001799ec
```
