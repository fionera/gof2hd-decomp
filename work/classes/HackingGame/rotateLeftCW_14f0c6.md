# HackingGame::rotateLeftCW
elf 0x14f0c6 body 34
Sig: undefined __thiscall rotateLeftCW(HackingGame * this, int * param_1)

## decompile
```c

/* HackingGame::rotateLeftCW(int*) */

void __thiscall HackingGame::rotateLeftCW(HackingGame *this,int *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *param_1;
  iVar1 = param_1[1];
  *param_1 = param_1[3];
  param_1[1] = iVar2;
  param_1[3] = param_1[4];
  param_1[4] = iVar1;
  this[0x128] = (HackingGame)0x1;
  *(undefined4 *)(this + 300) = 0;
  return;
}

```

## target disasm
```
  0015f0c6: push {r7,lr}
  0015f0c8: mov r7,sp
  0015f0ca: ldrd lr,r12,[r1,#0x0]
  0015f0ce: ldrd r3,r2,[r1,#0xc]
  0015f0d2: strd r3,lr,[r1,#0x0]
  0015f0d6: strd r2,r12,[r1,#0xc]
  0015f0da: movs r1,#0x1
  0015f0dc: strb.w r1,[r0,#0x128]
  0015f0e0: movs r1,#0x0
  0015f0e2: str.w r1,[r0,#0x12c]
  0015f0e6: pop {r7,pc}
```
