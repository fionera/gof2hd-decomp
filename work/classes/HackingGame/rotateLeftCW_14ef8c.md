# HackingGame::rotateLeftCW
elf 0x14ef8c body 56
Sig: undefined __stdcall rotateLeftCW(bool param_1)

## decompile
```c

/* HackingGame::rotateLeftCW(bool) */

void HackingGame::rotateLeftCW(bool param_1)

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
      FModSound::play(**(int **)(DAT_0015efc4 + 0x15efa8),(Vector *)0x8e2,(Vector *)0x0,in_s0);
    }
    (*(code *)(DAT_001ac4f4 + 0x1ac4f8))(uVar1,uVar1 + 0x34,local_18,in_r3);
    return;
  }
  return;
}

```

## target disasm
```
  0015ef8c: push {r2,r3,r4,r6,r7,lr}
  0015ef8e: add r7,sp,#0x10
  0015ef90: mov r4,r0
  0015ef92: ldr.w r0,[r0,#0x130]
  0015ef96: cbz r0,0x0015ef9a
  0015ef98: pop {r2,r3,r4,r6,r7,pc}
  0015ef9a: cbz r1,0x0015efb4
  0015ef9c: ldr r0,[0x0015efc4]
  0015ef9e: movs r1,#0x0
  0015efa0: movs r2,#0x0
  0015efa2: movs r3,#0x0
  0015efa4: add r0,pc
  0015efa6: ldr r0,[r0,#0x0]
  0015efa8: ldr r0,[r0,#0x0]
  0015efaa: str r1,[sp,#0x0]
  0015efac: movw r1,#0x8e2
  0015efb0: blx 0x00071548
  0015efb4: add.w r1,r4,#0x34
  0015efb8: mov r0,r4
  0015efba: pop.w {r2,r3,r4,r6,r7,lr}
  0015efbe: b.w 0x001ac4e8
  001ac4e8: bx pc
```
