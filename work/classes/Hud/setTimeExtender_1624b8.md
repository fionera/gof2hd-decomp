# Hud::setTimeExtender
elf 0x1624b8 body 36
Sig: undefined __stdcall setTimeExtender(bool param_1, bool param_2, bool param_3, bool param_4)

## decompile
```c

/* Hud::setTimeExtender(bool, bool, bool, bool) */

void Hud::setTimeExtender(bool param_1,bool param_2,bool param_3,bool param_4)

{
  bool *pbVar1;
  bool in_stack_00000000;
  
  pbVar1 = (bool *)(uint)param_1;
  pbVar1[0x280] = param_4;
  *pbVar1 = param_2;
  pbVar1[0x281] = in_stack_00000000;
  if ((param_3) && (param_4)) {
    pbVar1[0x4bc] = true;
    pbVar1[0x4bd] = false;
    pbVar1[0x4be] = false;
    pbVar1[0x4bf] = false;
    pbVar1[0x4c0] = true;
    pbVar1[0x4c1] = true;
    pbVar1[0x4c2] = false;
    pbVar1[0x4c3] = false;
  }
  return;
}

```

## target disasm
```
  001724b8: cmp r2,#0x0
  001724ba: ldr.w r12,[sp,#0x0]
  001724be: strb.w r3,[r0,#0x280]
  001724c2: strb r1,[r0,#0x0]
  001724c4: strb.w r12,[r0,#0x281]
  001724c8: beq 0x001724da
  001724ca: cbz r3,0x001724da
  001724cc: movs r1,#0x50
  001724ce: mov.w r2,#0x7d0
  001724d2: str.w r1,[r0,#0x4bc]
  001724d6: str.w r2,[r0,#0x4c0]
  001724da: bx lr
```
