# Route::Route
elf 0x11bc56 body 132
Sig: undefined __thiscall Route(Route * this, int * param_1, Array * param_2, int * param_3, int param_4)

## decompile
```c

/* Route::Route(int*, Array<KIPlayer*>*, int*, int) */

Route * __thiscall Route::Route(Route *this,int *param_1,Array *param_2,int *param_3,int param_4)

{
  Array<Waypoint*> *this_00;
  void *pvVar1;
  int iVar2;
  Waypoint *this_01;
  int iVar3;
  
  this[4] = (Route)0x0;
  *(undefined4 *)this = 0;
  this_00 = operator_new(0xc);
  Array<Waypoint*>::Array(this_00);
  *(Array<Waypoint*> **)(this + 0xc) = this_00;
  pvVar1 = operator_new(0xc);
  Array<int>::Array();
  *(Array **)(this + 0x10) = param_2;
  *(void **)(this + 0x14) = pvVar1;
  for (iVar3 = 0; iVar3 < param_4; iVar3 = iVar3 + 3) {
    iVar2 = *param_3;
    param_3 = param_3 + 1;
    ArrayAdd<int>(iVar2,*(Array **)(this + 0x14));
  }
  for (iVar3 = 0; iVar3 < param_4; iVar3 = iVar3 + 3) {
    this_01 = operator_new(0x138);
    Waypoint::Waypoint(this_01,param_1[iVar3],param_1[iVar3 + 1],param_1[iVar3 + 2],this);
    ArrayAdd<Waypoint*>(this_01,*(Array **)(this + 0xc));
  }
  return this;
}

```

## target disasm
```
  0012bc56: push {r4,r5,r6,r7,lr}
  0012bc58: add r7,sp,#0xc
  0012bc5a: push {r7,r8,r9,r10,r11}
  0012bc5e: mov r4,r0
  0012bc60: movs r0,#0x0
  0012bc62: strb r0,[r4,#0x4]
  0012bc64: mov r6,r3
  0012bc66: str r0,[r4,#0x0]
  0012bc68: movs r0,#0xc
  0012bc6a: mov r8,r2
  0012bc6c: mov r10,r1
  0012bc6e: blx 0x0006eb24
  0012bc72: mov r9,r0
  0012bc74: blx 0x00075f64
  0012bc78: movs r0,#0xc
  0012bc7a: str.w r9,[r4,#0xc]
  0012bc7e: blx 0x0006eb24
  0012bc82: mov r9,r0
  0012bc84: blx 0x000701f8
  0012bc88: ldr.w r11,[r7,#0x8]
  0012bc8c: movs r5,#0x0
  0012bc8e: strd r8,r9,[r4,#0x10]
  0012bc92: b 0x0012bc9e
  0012bc94: ldr r1,[r4,#0x14]
  0012bc96: ldmia r6!,{r0}
  0012bc98: blx 0x0007021c
  0012bc9c: adds r5,#0x3
  0012bc9e: cmp r5,r11
  0012bca0: blt 0x0012bc94
  0012bca2: movs r5,#0x0
  0012bca4: b 0x0012bcce
  0012bca6: mov.w r0,#0x138
  0012bcaa: blx 0x0006eb24
  0012bcae: ldr.w r1,[r10,r5,lsl #0x2]
  0012bcb2: mov r6,r0
  0012bcb4: add.w r0,r10,r5, lsl #0x2
  0012bcb8: ldrd r2,r3,[r0,#0x4]
  0012bcbc: mov r0,r6
  0012bcbe: str r4,[sp,#0x0]
  0012bcc0: blx 0x00073e4c
  0012bcc4: ldr r1,[r4,#0xc]
  0012bcc6: mov r0,r6
  0012bcc8: blx 0x00075f70
  0012bccc: adds r5,#0x3
  0012bcce: cmp r5,r11
  0012bcd0: blt 0x0012bca6
  0012bcd2: mov r0,r4
  0012bcd4: pop.w {r3,r8,r9,r10,r11}
  0012bcd8: pop {r4,r5,r6,r7,pc}
```
