# Layout::showMissionRewardMessage
elf 0xd4d40 body 46
Sig: undefined __stdcall showMissionRewardMessage(int param_1, bool param_2)

## decompile
```c

/* Layout::showMissionRewardMessage(int, bool) */

void Layout::showMissionRewardMessage(int param_1,bool param_2)

{
  int iVar1;
  undefined1 in_r2;
  int *piVar2;
  float in_s0;
  
  iVar1 = DAT_000e4d70;
  if (param_2 != 0) {
    *(undefined1 *)(param_1 + 0x2ed) = in_r2;
    *(undefined1 *)(param_1 + 0x2ec) = 1;
    piVar2 = *(int **)(iVar1 + 0xe4d52);
    *(undefined4 *)(param_1 + 0x3d0) = 0;
    *(uint *)(param_1 + 0x3d4) = (uint)param_2;
    FModSound::play(*piVar2,(Vector *)0x24,(Vector *)0x0,in_s0);
    return;
  }
  return;
}

```

## target disasm
```
  000e4d40: cbz r1,0x000e4d6c
  000e4d42: push {r5,r6,r7,lr}
  000e4d44: add r7,sp,#0x8
  000e4d46: ldr r3,[0x000e4d70]
  000e4d48: strb.w r2,[r0,#0x2ed]
  000e4d4c: movs r2,#0x1
  000e4d4e: add r3,pc
  000e4d50: strb.w r2,[r0,#0x2ec]
  000e4d54: ldr r2,[r3,#0x0]
  000e4d56: movs r3,#0x0
  000e4d58: strd r3,r1,[r0,#0x3d0]
  000e4d5c: movs r1,#0x24
  000e4d5e: str r3,[sp,#0x0]
  000e4d60: movs r3,#0x0
  000e4d62: ldr r0,[r2,#0x0]
  000e4d64: movs r2,#0x0
  000e4d66: blx 0x00071548
  000e4d6a: pop {r2,r3,r7,pc}
  000e4d6c: bx lr
```
