# PlayerStaticFar::getInitPosition
elf 0x11c4ee body 14
Sig: undefined __thiscall getInitPosition(PlayerStaticFar * this, Vector param_1)

## decompile
```c

/* PlayerStaticFar::getInitPosition(AbyssEngine::AEMath::Vector) */

void __thiscall PlayerStaticFar::getInitPosition(PlayerStaticFar *this,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(param_2 + 0x5c);
  uVar1 = *(undefined4 *)(param_2 + 0x60);
  *(undefined4 *)this = *(undefined4 *)(param_2 + 0x58);
  *(undefined4 *)(this + 4) = uVar2;
  *(undefined4 *)(this + 8) = uVar1;
  return;
}

```

## target disasm
```
  0012c4ee: ldrd r2,r3,[r1,#0x58]
  0012c4f2: ldr r1,[r1,#0x60]
  0012c4f4: strd r2,r3,[r0,#0x0]
  0012c4f8: str r1,[r0,#0x8]
  0012c4fa: bx lr
```
