# PlayerFighter::setShootError
elf 0xdccf4 body 14
Sig: undefined __thiscall setShootError(PlayerFighter * this, int param_1)

## decompile
```c

/* PlayerFighter::setShootError(int) */

void __thiscall PlayerFighter::setShootError(PlayerFighter *this,int param_1)

{
  uint in_fpscr;
  undefined4 uVar1;
  
  uVar1 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 0x1a8) = uVar1;
  return;
}

```

## target disasm
```
  000eccf4: vmov s0,r1
  000eccf8: vcvt.f32.s32 s0,s0
  000eccfc: vstr.32 s0,[r0,#0x1a8]
  000ecd00: bx lr
```
