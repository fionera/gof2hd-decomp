# PlayerCreature::rage
elf 0x11cce8 body 72
Sig: undefined __thiscall rage(PlayerCreature * this, int param_1)

## decompile
```c

/* PlayerCreature::rage(int) */

void __thiscall PlayerCreature::rage(PlayerCreature *this,int param_1)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  
  fVar1 = (float)VectorSignedToFloat(param_1,(byte)(in_fpscr >> 0x16) & 3);
  fVar1 = fVar1 / DAT_0012cd30;
  fVar2 = (float)VectorSignedToFloat(*(undefined4 *)
                                      (DAT_0012cd34 + 0x12cd02 + *(int *)(this + 0xac) * 4),
                                     (byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(this + 0x130) = 0;
  *(undefined2 *)(this + 0x128) = 0x101;
  *(float *)(this + 300) = (fVar1 + 1.0) * (fVar2 + 1.0);
  return;
}

```

## target disasm
```
  0012cce8: vmov s0,r1
  0012ccec: vldr.32 s2,[pc,#0x40]
  0012ccf0: vmov.f32 s4,0x3f800000
  0012ccf4: vcvt.f32.s32 s0,s0
  0012ccf8: ldr r1,[0x0012cd34]
  0012ccfa: ldr.w r2,[r0,#0xac]
  0012ccfe: add r1,pc
  0012cd00: add.w r1,r1,r2, lsl #0x2
  0012cd04: vdiv.f32 s0,s0,s2
  0012cd08: vldr.32 s2,[r1]
  0012cd0c: vadd.f32 s0,s0,s4
  0012cd10: movs r1,#0x0
  0012cd12: vcvt.f32.s32 s2,s2
  0012cd16: str.w r1,[r0,#0x130]
  0012cd1a: movw r1,#0x101
  0012cd1e: strh.w r1,[r0,#0x128]
  0012cd22: vadd.f32 s2,s2,s4
  0012cd26: vmul.f32 s0,s0,s2
  0012cd2a: vstr.32 s0,[r0,#0x12c]
  0012cd2e: bx lr
```
