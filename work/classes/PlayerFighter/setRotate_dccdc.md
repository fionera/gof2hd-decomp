# PlayerFighter::setRotate
elf 0xdccdc body 24
Sig: undefined __thiscall setRotate(PlayerFighter * this, int param_1)

## decompile
```c

/* PlayerFighter::setRotate(int) */

void __thiscall PlayerFighter::setRotate(PlayerFighter *this,int param_1)

{
  uint in_fpscr;
  undefined4 uVar1;
  
  uVar1 = VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  this[0x13d] = (PlayerFighter)0x0;
  *(undefined4 *)(this + 0x1a4) = uVar1;
  *(undefined4 *)(this + 0x1f0) = uVar1;
  return;
}

```

## target disasm
```
  000eccdc: vmov s0,r1
  000ecce0: movs r1,#0x0
  000ecce2: vcvt.f32.s32 s0,s0
  000ecce6: strb.w r1,[r0,#0x13d]
  000eccea: vstr.32 s0,[r0,#0x1a4]
  000eccee: vstr.32 s0,[r0,#0x1f0]
  000eccf2: bx lr
```
