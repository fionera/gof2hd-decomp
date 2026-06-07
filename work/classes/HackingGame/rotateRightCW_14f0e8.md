# HackingGame::rotateRightCW
elf 0x14f0e8 body 34
Sig: undefined __thiscall rotateRightCW(HackingGame * this, int * param_1)

## decompile
```c

/* HackingGame::rotateRightCW(int*) */

void __thiscall HackingGame::rotateRightCW(HackingGame *this,int *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_1[1];
  iVar1 = param_1[2];
  param_1[1] = param_1[4];
  param_1[2] = iVar2;
  param_1[4] = param_1[5];
  param_1[5] = iVar1;
  this[0x129] = (HackingGame)0x1;
  *(undefined4 *)(this + 300) = 0;
  return;
}

```

## target disasm
```
  0015f0e8: push {r7,lr}
  0015f0ea: mov r7,sp
  0015f0ec: ldrd lr,r12,[r1,#0x4]
  0015f0f0: ldrd r3,r2,[r1,#0x10]
  0015f0f4: strd r3,lr,[r1,#0x4]
  0015f0f8: strd r2,r12,[r1,#0x10]
  0015f0fc: movs r1,#0x1
  0015f0fe: strb.w r1,[r0,#0x129]
  0015f102: movs r1,#0x0
  0015f104: str.w r1,[r0,#0x12c]
  0015f108: pop {r7,pc}
```
