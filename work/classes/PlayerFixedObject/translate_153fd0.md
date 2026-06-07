# PlayerFixedObject::translate
elf 0x153fd0 body 70
Sig: undefined __thiscall translate(PlayerFixedObject * this, Vector * param_1)

## decompile
```c

/* PlayerFixedObject::translate(AbyssEngine::AEMath::Vector const&) */

void __thiscall PlayerFixedObject::translate(PlayerFixedObject *this,Vector *param_1)

{
  uint in_fpscr;
  float fVar1;
  float fVar2;
  float fVar3;
  
  fVar1 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x178),(byte)(in_fpscr >> 0x16) & 3);
  fVar2 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x17c),(byte)(in_fpscr >> 0x16) & 3);
  fVar3 = (float)VectorSignedToFloat(*(undefined4 *)(this + 0x180),(byte)(in_fpscr >> 0x16) & 3);
                    /* WARNING: Could not recover jumptable at 0x00164014. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(int *)this + 0x48))
            (this,*(float *)param_1 + fVar1,*(float *)(param_1 + 4) + fVar2,
             *(float *)(param_1 + 8) + fVar3);
  return;
}

```

## target disasm
```
  00163fd0: vldr.32 s0,[r0,#0x178]
  00163fd4: vldr.32 s2,[r0,#0x17c]
  00163fd8: vldr.32 s4,[r0,#0x180]
  00163fdc: vcvt.f32.s32 s0,s0
  00163fe0: vcvt.f32.s32 s2,s2
  00163fe4: vcvt.f32.s32 s4,s4
  00163fe8: vldr.32 s6,[r1]
  00163fec: vldr.32 s8,[r1,#0x4]
  00163ff0: vldr.32 s10,[r1,#0x8]
  00163ff4: ldr.w r12,[r0,#0x0]
  00163ff8: vadd.f32 s0,s6,s0
  00163ffc: vadd.f32 s2,s8,s2
  00164000: ldr.w r12,[r12,#0x48]
  00164004: vadd.f32 s4,s10,s4
  00164008: vmov r1,s0
  0016400c: vmov r2,s2
  00164010: vmov r3,s4
  00164014: bx r12
```
