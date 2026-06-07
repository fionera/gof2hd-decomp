# Route::Route
elf 0x11bb74 body 142
Sig: undefined __thiscall Route(Route * this, int * param_1, int param_2)

## decompile
```c

/* Route::Route(int*, int) */

Route * __thiscall Route::Route(Route *this,int *param_1,int param_2)

{
  Array<Waypoint*> *this_00;
  Array<KIPlayer*> *this_01;
  void *pvVar1;
  uint uVar2;
  Waypoint *this_02;
  int iVar3;
  
  this[4] = (Route)0x0;
  *(undefined4 *)this = 0;
  this_00 = operator_new(0xc);
  Array<Waypoint*>::Array(this_00);
  *(Array<Waypoint*> **)(this + 0xc) = this_00;
  this_01 = operator_new(0xc);
  Array<KIPlayer*>::Array(this_01);
  *(Array<KIPlayer*> **)(this + 0x10) = this_01;
  pvVar1 = operator_new(0xc);
  Array<int>::Array();
  *(void **)(this + 0x14) = pvVar1;
  uVar2 = __aeabi_idiv(param_2,3);
  ArraySetLength<KIPlayer*>(uVar2,*(Array **)(this + 0x10));
  ArraySetLength<int>(uVar2,*(Array **)(this + 0x14));
  for (iVar3 = 0; iVar3 < param_2; iVar3 = iVar3 + 3) {
    this_02 = operator_new(0x138);
    Waypoint::Waypoint(this_02,param_1[iVar3],param_1[iVar3 + 1],param_1[iVar3 + 2],this);
    ArrayAdd<Waypoint*>(this_02,*(Array **)(this + 0xc));
  }
  return this;
}

```

## target disasm
```
  0012bb74: push {r4,r5,r6,r7,lr}
  0012bb76: add r7,sp,#0xc
  0012bb78: push {r6,r7,r8,r9,r11}
  0012bb7c: mov r5,r0
  0012bb7e: movs r0,#0x0
  0012bb80: strb r0,[r5,#0x4]
  0012bb82: mov r8,r2
  0012bb84: str r0,[r5,#0x0]
  0012bb86: movs r0,#0xc
  0012bb88: mov r9,r1
  0012bb8a: blx 0x0006eb24
  0012bb8e: mov r4,r0
  0012bb90: blx 0x00075f64
  0012bb94: movs r0,#0xc
  0012bb96: str r4,[r5,#0xc]
  0012bb98: blx 0x0006eb24
  0012bb9c: mov r4,r0
  0012bb9e: blx 0x00073d08
  0012bba2: movs r0,#0xc
  0012bba4: str r4,[r5,#0x10]
  0012bba6: blx 0x0006eb24
  0012bbaa: mov r4,r0
  0012bbac: blx 0x000701f8
  0012bbb0: mov r0,r8
  0012bbb2: movs r1,#0x3
  0012bbb4: str r4,[r5,#0x14]
  0012bbb6: blx 0x0007198c
  0012bbba: ldr r1,[r5,#0x10]
  0012bbbc: mov r4,r0
  0012bbbe: blx 0x00073d14
  0012bbc2: ldr r1,[r5,#0x14]
  0012bbc4: mov r0,r4
  0012bbc6: blx 0x00071a4c
  0012bbca: movs r6,#0x0
  0012bbcc: b 0x0012bbf6
  0012bbce: mov.w r0,#0x138
  0012bbd2: blx 0x0006eb24
  0012bbd6: ldr.w r1,[r9,r6,lsl #0x2]
  0012bbda: mov r4,r0
  0012bbdc: add.w r0,r9,r6, lsl #0x2
  0012bbe0: ldrd r2,r3,[r0,#0x4]
  0012bbe4: mov r0,r4
  0012bbe6: str r5,[sp,#0x0]
  0012bbe8: blx 0x00073e4c
  0012bbec: ldr r1,[r5,#0xc]
  0012bbee: mov r0,r4
  0012bbf0: blx 0x00075f70
  0012bbf4: adds r6,#0x3
  0012bbf6: cmp r6,r8
  0012bbf8: blt 0x0012bbce
  0012bbfa: mov r0,r5
  0012bbfc: pop.w {r2,r3,r8,r9,r11}
  0012bc00: pop {r4,r5,r6,r7,pc}
```
