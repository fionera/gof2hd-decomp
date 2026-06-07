# PlayerEgo::changeThrust
elf 0xa2214 body 90
Sig: undefined __thiscall changeThrust(PlayerEgo * this, float param_1)

## decompile
```c

/* PlayerEgo::changeThrust(float) */

void __thiscall PlayerEgo::changeThrust(PlayerEgo *this,float param_1)

{
  float in_r1;
  int *piVar1;
  int *piVar2;
  int *piVar3;
  
  piVar1 = (int *)((float)this[5].field_8 + in_r1);
  piVar3 = (int *)&LAB_3f800000;
  if ((int)((uint)((float)piVar1 < 1.0) << 0x1f) < 0) {
    piVar3 = DAT_000b2270;
  }
  if ((int)((uint)((float)piVar1 < 0.0) << 0x1f) < 0) {
    piVar3 = DAT_000b2270;
  }
  piVar2 = piVar3;
  if ((int)((uint)((float)piVar1 < 1.0) << 0x1f) < 0) {
    piVar2 = piVar1;
  }
  if ((int)((uint)((float)piVar1 < 0.0) << 0x1f) < 0) {
    piVar2 = piVar3;
  }
  this[5].field_8 = piVar2;
  return;
}

```

## target disasm
```
  000b2214: vmov s0,r1
  000b2218: vldr.32 s2,[r0,#0xbc]
  000b221c: vmov.f32 s4,0x3f800000
  000b2220: vadd.f32 s0,s2,s0
  000b2224: vldr.32 s2,[pc,#0x48]
  000b2228: vmov.f32 s6,s4
  000b222c: vcmpe.f32 s0,s4
  000b2230: vmrs apsr,fpscr
  000b2234: vcmpe.f32 s0,#0
  000b2238: it mi
  000b223a: vmov.mi.f32 s6,s2
  000b223e: vmrs apsr,fpscr
  000b2242: vcmpe.f32 s0,s4
  000b2246: it mi
  000b2248: vmov.mi.f32 s6,s2
  000b224c: vmrs apsr,fpscr
  000b2250: vmov.f32 s2,s6
  000b2254: vcmpe.f32 s0,#0
  000b2258: it mi
  000b225a: vmov.mi.f32 s2,s0
  000b225e: vmrs apsr,fpscr
  000b2262: it mi
  000b2264: vmov.mi.f32 s2,s6
  000b2268: vstr.32 s2,[r0,#0xbc]
  000b226c: bx lr
```
