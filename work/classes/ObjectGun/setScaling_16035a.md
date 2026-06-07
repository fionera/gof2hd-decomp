# ObjectGun::setScaling
elf 0x16035a body 26
Sig: undefined __stdcall setScaling(int param_1, int param_2, int param_3)

## decompile
```c

/* ObjectGun::setScaling(int, int, int) */

void ObjectGun::setScaling(int param_1,int param_2,int param_3)

{
  uint in_fpscr;
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = VectorSignedToFloat(param_3,(byte)(in_fpscr >> 0x16) & 3);
  uVar2 = VectorSignedToFloat(param_2,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(param_1 + 0x3c) = uVar2;
  *(undefined4 *)(param_1 + 0x40) = uVar1;
  return;
}

```

## target disasm
```
  0017035a: vmov s0,r2
  0017035e: vmov s2,r1
  00170362: vcvt.f32.s32 s0,s0
  00170366: vcvt.f32.s32 s2,s2
  0017036a: vstr.32 s2,[r0,#0x3c]
  0017036e: vstr.32 s0,[r0,#0x40]
  00170372: bx lr
```
