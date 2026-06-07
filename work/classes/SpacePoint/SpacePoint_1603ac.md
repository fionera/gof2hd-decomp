# SpacePoint::SpacePoint
elf 0x1603ac body 58
Sig: undefined __thiscall SpacePoint(SpacePoint * this, int param_1, Vector * param_2, Vector * param_3, int param_4)

## decompile
```c

/* SpacePoint::SpacePoint(int, AbyssEngine::AEMath::Vector const&, AbyssEngine::AEMath::Vector
   const&, int) */

SpacePoint * __thiscall
SpacePoint::SpacePoint(SpacePoint *this,int param_1,Vector *param_2,Vector *param_3,int param_4)

{
  *(undefined4 *)(this + 0x10) = 0;
  *(int *)(this + 0x18) = param_1;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this + 0x14) = 0;
  AbyssEngine::AEMath::Vector::operator=((Vector *)this,param_2);
  AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0xc),param_3);
  this[0x1c] = (SpacePoint)0x1;
  *(int *)(this + 0x20) = param_4;
  return this;
}

```

## target disasm
```
  001703ac: push {r4,r5,r7,lr}
  001703ae: add r7,sp,#0x8
  001703b0: mov r5,r0
  001703b2: movs r0,#0x0
  001703b4: mov r4,r3
  001703b6: str r0,[r5,#0x10]
  001703b8: str r1,[r5,#0x18]
  001703ba: vmov.i32 q8,#0x0
  001703be: movs r1,#0x14
  001703c0: mov r3,r5
  001703c2: vst1.32 {d16,d17},[r3],r1
  001703c6: mov r1,r2
  001703c8: str r0,[r3,#0x0]
  001703ca: mov r0,r5
  001703cc: blx 0x0006eb3c
  001703d0: add.w r0,r5,#0xc
  001703d4: mov r1,r4
  001703d6: blx 0x0006eb3c
  001703da: movs r1,#0x1
  001703dc: ldr r0,[r7,#0x8]
  001703de: strb r1,[r5,#0x1c]
  001703e0: str r0,[r5,#0x20]
  001703e2: mov r0,r5
  001703e4: pop {r4,r5,r7,pc}
```
