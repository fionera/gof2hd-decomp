# PlayerWormHole::getPosition
elf 0xa57a0 body 38
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* PlayerWormHole::getPosition() */

void PlayerWormHole::getPosition(void)

{
  undefined4 *in_r0;
  int in_r1;
  uint in_fpscr;
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  uVar2 = VectorSignedToFloat(*(undefined4 *)(in_r1 + 0x128),(byte)(in_fpscr >> 0x16) & 3);
  uVar1 = VectorSignedToFloat(*(undefined4 *)(in_r1 + 0x124),(byte)(in_fpscr >> 0x16) & 3);
  uVar3 = VectorSignedToFloat(*(undefined4 *)(in_r1 + 300),(byte)(in_fpscr >> 0x16) & 3);
  *in_r0 = uVar1;
  in_r0[1] = uVar2;
  in_r0[2] = uVar3;
  return;
}

```

## target disasm
```
  000b57a0: vldr.32 s0,[r1,#0x124]
  000b57a4: vldr.32 s2,[r1,#0x128]
  000b57a8: vldr.32 s4,[r1,#0x12c]
  000b57ac: vcvt.f32.s32 s2,s2
  000b57b0: vcvt.f32.s32 s0,s0
  000b57b4: vcvt.f32.s32 s4,s4
  000b57b8: vstr.32 s0,[r0]
  000b57bc: vstr.32 s2,[r0,#0x4]
  000b57c0: vstr.32 s4,[r0,#0x8]
  000b57c4: bx lr
```
