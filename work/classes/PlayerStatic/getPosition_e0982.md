# PlayerStatic::getPosition
elf 0xe0982 body 54
Sig: undefined __stdcall getPosition(void)

## decompile
```c

/* PlayerStatic::getPosition() */

void PlayerStatic::getPosition(void)

{
  undefined4 *in_r0;
  int in_r1;
  uint in_fpscr;
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  if (*(int *)(in_r1 + 8) != 0) {
    AEGeometry::getPosition();
    return;
  }
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
  000f0982: ldr r2,[r1,#0x8]
  000f0984: cbz r2,0x000f0992
  000f0986: push {r7,lr}
  000f0988: mov r7,sp
  000f098a: mov r1,r2
  000f098c: blx 0x000720b8
  000f0990: pop {r7,pc}
  000f0992: vldr.32 s0,[r1,#0x124]
  000f0996: vldr.32 s2,[r1,#0x128]
  000f099a: vldr.32 s4,[r1,#0x12c]
  000f099e: vcvt.f32.s32 s2,s2
  000f09a2: vcvt.f32.s32 s0,s0
  000f09a6: vcvt.f32.s32 s4,s4
  000f09aa: vstr.32 s0,[r0]
  000f09ae: vstr.32 s2,[r0,#0x4]
  000f09b2: vstr.32 s4,[r0,#0x8]
  000f09b6: bx lr
```
