# Gun::setOffset
elf 0x1522e4 body 60
Sig: undefined __thiscall setOffset(Gun * this, Vector * param_1)

## decompile
```c

/* Gun::setOffset(AbyssEngine::AEMath::Vector*) */

undefined8 __thiscall Gun::setOffset(Gun *this,Vector *param_1)

{
  int *piVar1;
  undefined4 local_20;
  undefined4 uStack_1c;
  float local_18;
  int local_14;
  
  piVar1 = *(int **)(DAT_00162324 + 0x1622f4);
  local_14 = *piVar1;
  local_20 = *(undefined4 *)param_1;
  uStack_1c = *(undefined4 *)(param_1 + 4);
  local_18 = *(float *)(param_1 + 8) + DAT_00162320;
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0x7c),(Vector *)&local_20);
  if (*piVar1 == local_14) {
    return CONCAT44(uStack_1c,local_20);
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  001622e4: push {r0,r1,r2,r3,r4,r6,r7,lr}
  001622e6: add r7,sp,#0x18
  001622e8: ldr r2,[0x00162324]
  001622ea: adds r0,#0x7c
  001622ec: vldr.32 s0,[pc,#0x30]
  001622f0: add r2,pc
  001622f2: ldr r4,[r2,#0x0]
  001622f4: ldr r2,[r4,#0x0]
  001622f6: str r2,[sp,#0xc]
  001622f8: vldr.32 s2,[r1,#0x8]
  001622fc: ldrd r2,r1,[r1,#0x0]
  00162300: vadd.f32 s0,s2,s0
  00162304: strd r2,r1,[sp,#0x0]
  00162308: mov r1,sp
  0016230a: vstr.32 s0,[sp,#0x8]
  0016230e: blx 0x0006eb3c
  00162312: ldr r0,[sp,#0xc]
  00162314: ldr r1,[r4,#0x0]
  00162316: subs r0,r1,r0
  00162318: it eq
  0016231a: pop.eq {r0,r1,r2,r3,r4,r6,r7,pc}
  0016231c: blx 0x0006e824
```
