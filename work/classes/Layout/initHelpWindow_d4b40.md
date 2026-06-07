# Layout::initHelpWindow
elf 0xd4b40 body 106
Sig: undefined __stdcall initHelpWindow(String param_1)

## decompile
```c

/* Layout::initHelpWindow(AbyssEngine::String) */

void Layout::initHelpWindow(float param_1,undefined1 *param_2,String *param_3)

{
  ChoiceWindow *pCVar1;
  String *pSVar2;
  
  if (*(int *)(param_2 + 0x3c4) == 0) {
    pCVar1 = operator_new(0x5c);
    param_1 = (float)ChoiceWindow::ChoiceWindow(pCVar1);
    *(ChoiceWindow **)(param_2 + 0x3c4) = pCVar1;
  }
  FModSound::play(**(int **)(DAT_000e4bb8 + 0xe4b70),(Vector *)0x7e,(Vector *)0x0,param_1);
  pCVar1 = *(ChoiceWindow **)(param_2 + 0x3c4);
  pSVar2 = (String *)GameText::getText(**(int **)(DAT_000e4bbc + 0xe4b88));
  ChoiceWindow::set(pCVar1,pSVar2,param_3,false);
  *param_2 = 1;
  param_2[0x3c0] = 0;
  return;
}

```

## target disasm
```
  000e4b40: push {r4,r5,r6,r7,lr}
  000e4b42: add r7,sp,#0xc
  000e4b44: push.w r8
  000e4b48: sub sp,#0x8
  000e4b4a: mov r4,r0
  000e4b4c: ldr.w r0,[r0,#0x3c4]
  000e4b50: mov r8,r1
  000e4b52: cbnz r0,0x000e4b64
  000e4b54: movs r0,#0x5c
  000e4b56: blx 0x0006eb24
  000e4b5a: mov r6,r0
  000e4b5c: blx 0x00074584
  000e4b60: str.w r6,[r4,#0x3c4]
  000e4b64: ldr r0,[0x000e4bb8]
  000e4b66: movs r5,#0x0
  000e4b68: movs r1,#0x7e
  000e4b6a: movs r2,#0x0
  000e4b6c: add r0,pc
  000e4b6e: movs r3,#0x0
  000e4b70: str r5,[sp,#0x0]
  000e4b72: ldr r0,[r0,#0x0]
  000e4b74: ldr r0,[r0,#0x0]
  000e4b76: blx 0x00071548
  000e4b7a: ldr r0,[0x000e4bbc]
  000e4b7c: movw r1,#0x187
  000e4b80: ldr.w r6,[r4,#0x3c4]
  000e4b84: add r0,pc
  000e4b86: ldr r0,[r0,#0x0]
  000e4b88: ldr r0,[r0,#0x0]
  000e4b8a: blx 0x00072f70
  000e4b8e: mov r1,r0
  000e4b90: mov r0,r6
  000e4b92: mov r2,r8
  000e4b94: movs r3,#0x0
  000e4b96: blx 0x00074ea8
  000e4b9a: movs r0,#0x1
  000e4b9c: strb r0,[r4,#0x0]
  000e4b9e: strb.w r5,[r4,#0x3c0]
  000e4ba2: add sp,#0x8
  000e4ba4: pop.w r8
  000e4ba8: pop {r4,r5,r6,r7,pc}
```
