# HackingGame::rotateRightCW
elf 0x14ef50 body 56
Sig: undefined __stdcall rotateRightCW(bool param_1)

## decompile
```c

/* HackingGame::rotateRightCW(bool) */

void HackingGame::rotateRightCW(bool param_1)

{
  uint uVar1;
  int in_r1;
  undefined4 in_r2;
  undefined4 in_r3;
  float in_s0;
  undefined4 local_18;
  
  uVar1 = (uint)param_1;
  if (*(int *)(uVar1 + 0x130) == 0) {
    local_18 = in_r2;
    if (in_r1 != 0) {
      local_18 = 0;
      FModSound::play(**(int **)(DAT_0015ef88 + 0x15ef6c),(Vector *)0x8e2,(Vector *)0x0,in_s0);
    }
    (*(code *)(DAT_001ac4e4 + 0x1ac4e8))(uVar1,uVar1 + 0x34,local_18,in_r3);
    return;
  }
  return;
}

```

## target disasm
```
  0015ef50: push {r2,r3,r4,r6,r7,lr}
  0015ef52: add r7,sp,#0x10
  0015ef54: mov r4,r0
  0015ef56: ldr.w r0,[r0,#0x130]
  0015ef5a: cbz r0,0x0015ef5e
  0015ef5c: pop {r2,r3,r4,r6,r7,pc}
  0015ef5e: cbz r1,0x0015ef78
  0015ef60: ldr r0,[0x0015ef88]
  0015ef62: movs r1,#0x0
  0015ef64: movs r2,#0x0
  0015ef66: movs r3,#0x0
  0015ef68: add r0,pc
  0015ef6a: ldr r0,[r0,#0x0]
  0015ef6c: ldr r0,[r0,#0x0]
  0015ef6e: str r1,[sp,#0x0]
  0015ef70: movw r1,#0x8e2
  0015ef74: blx 0x00071548
  0015ef78: add.w r1,r4,#0x34
  0015ef7c: mov r0,r4
  0015ef7e: pop.w {r2,r3,r4,r6,r7,lr}
  0015ef82: b.w 0x001ac4d8
  001ac4d8: bx pc
```
