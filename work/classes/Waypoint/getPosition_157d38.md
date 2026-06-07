# Waypoint::getPosition
elf 0x157d38 body 38
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* Waypoint::getPosition() */

void Waypoint::getPosition(void)

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
  00167d38: vldr.32 s0,[r1,#0x124]
  00167d3c: vldr.32 s2,[r1,#0x128]
  00167d40: vldr.32 s4,[r1,#0x12c]
  00167d44: vcvt.f32.s32 s2,s2
  00167d48: vcvt.f32.s32 s0,s0
  00167d4c: vcvt.f32.s32 s4,s4
  00167d50: vstr.32 s0,[r0]
  00167d54: vstr.32 s2,[r0,#0x4]
  00167d58: vstr.32 s4,[r0,#0x8]
  00167d5c: bx lr
```
