# EaseInOut::EaseInOut
elf 0x71780 body 28
Sig: undefined __thiscall EaseInOut(EaseInOut * this)

## decompile
```c

/* AbyssEngine::EaseInOut::EaseInOut() */

EaseInOut * __thiscall AbyssEngine::EaseInOut::EaseInOut(EaseInOut *this)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = DAT_000817a0;
  uVar1 = DAT_0008179c;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = uVar2;
  *(undefined4 *)(this + 8) = uVar1;
  UpdateCurrentValue(this);
  return this;
}

```

## target disasm
```
  00081780: push {r4,r6,r7,lr}
  00081782: add r7,sp,#0x8
  00081784: ldr r1,[0x000817a0]
  00081786: mov r4,r0
  00081788: movs r2,#0x0
  0008178a: ldr r0,[0x0008179c]
  0008178c: strd r2,r1,[r4,#0x0]
  00081790: str r0,[r4,#0x8]
  00081792: mov r0,r4
  00081794: blx 0x0006f4f0
  00081798: mov r0,r4
  0008179a: pop {r4,r6,r7,pc}
```
