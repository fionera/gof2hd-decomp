# Route::setNewCoords
elf 0x11bdb8 body 44
Sig: undefined __stdcall setNewCoords(Vector param_1)

## decompile
```c

/* Route::setNewCoords(AbyssEngine::AEMath::Vector) */

void Route::setNewCoords(int param_1,float param_2,float param_3,float param_4)

{
  int iVar1;
  
  iVar1 = **(int **)(*(int *)(param_1 + 0xc) + 4);
  *(int *)(iVar1 + 0x124) = (int)param_2;
  *(int *)(iVar1 + 0x128) = (int)param_3;
  *(int *)(iVar1 + 300) = (int)param_4;
  return;
}

```

## target disasm
```
  0012bdb8: ldr r0,[r0,#0xc]
  0012bdba: vmov s0,r1
  0012bdbe: vmov s2,r2
  0012bdc2: vmov s4,r3
  0012bdc6: ldr r0,[r0,#0x4]
  0012bdc8: ldr r0,[r0,#0x0]
  0012bdca: vcvt.s32.f32 s2,s2
  0012bdce: vcvt.s32.f32 s0,s0
  0012bdd2: vcvt.s32.f32 s4,s4
  0012bdd6: vstr.32 s0,[r0,#0x124]
  0012bdda: vstr.32 s2,[r0,#0x128]
  0012bdde: vstr.32 s4,[r0,#0x12c]
  0012bde2: bx lr
```
