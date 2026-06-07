# PlayerJumpgate::setPosition
elf 0xa51c0 body 46
Sig: undefined __stdcall setPosition(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerJumpgate::setPosition(float, float, float) */

void PlayerJumpgate::setPosition(float param_1,float param_2,float param_3)

{
  int in_r0;
  float in_r1;
  float in_r2;
  float in_r3;
  
  *(int *)(in_r0 + 0x124) = (int)in_r1;
  *(int *)(in_r0 + 0x128) = (int)in_r2;
  *(int *)(in_r0 + 300) = (int)in_r3;
  (*(code *)(DAT_001abd74 + 0x1abd78))(*(undefined4 *)(in_r0 + 8));
  return;
}

```

## target disasm
```
  000b51c0: vmov s2,r2
  000b51c4: vmov s0,r1
  000b51c8: vmov s4,r3
  000b51cc: vcvt.s32.f32 s2,s2
  000b51d0: vcvt.s32.f32 s0,s0
  000b51d4: vcvt.s32.f32 s4,s4
  000b51d8: ldr.w r12,[r0,#0x8]
  000b51dc: vstr.32 s0,[r0,#0x124]
  000b51e0: vstr.32 s2,[r0,#0x128]
  000b51e4: vstr.32 s4,[r0,#0x12c]
  000b51e8: mov r0,r12
  000b51ea: b.w 0x001abd68
```
