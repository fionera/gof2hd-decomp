# TouchButton::touchedInside
elf 0x169938 body 138
Sig: undefined __thiscall touchedInside(TouchButton * this, int param_1, int param_2)

## decompile
```c

/* TouchButton::touchedInside(int, int) */

bool __thiscall TouchButton::touchedInside(TouchButton *this,int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = *(int *)(this + 0x78);
  if (*(int *)(this + 0x70) == 3) {
    iVar1 = *(int *)(**(int **)(DAT_001799c4 + 0x179950) + 0x38);
    if ((param_1 < iVar1 + iVar3 + -1) || (*(int *)(this + 0x90) + ((iVar3 + 1) - iVar1) <= param_1)
       ) {
      return false;
    }
    iVar1 = *(int *)(this + 0x7c);
    if (param_2 < iVar1 + -1) {
      return false;
    }
    iVar3 = *(int *)(this + 0x8c);
  }
  else {
    uVar2 = *(uint *)(this + 0xc0);
    if (param_1 < (int)((iVar3 + -1) - uVar2)) {
      return false;
    }
    if ((int)(*(int *)(this + 0x90) + iVar3 + uVar2 + 1) <= param_1) {
      return false;
    }
    if (param_2 < (int)(*(int *)(this + 0x7c) + ~uVar2)) {
      return false;
    }
    iVar3 = *(int *)(this + 0x8c);
    iVar1 = *(int *)(this + 0x7c) + uVar2;
  }
  return param_2 <= iVar3 + iVar1 + 1;
}

```

## target disasm
```
  00179938: push {r4,r6,r7,lr}
  0017993a: add r7,sp,#0x8
  0017993c: ldr.w lr,[r0,#0x78]
  00179940: ldr r3,[r0,#0x70]
  00179942: sub.w r4,lr,#0x1
  00179946: cmp r3,#0x3
  00179948: bne 0x00179980
  0017994a: ldr r3,[0x001799c4]
  0017994c: add r3,pc
  0017994e: ldr r3,[r3,#0x0]
  00179950: ldr r3,[r3,#0x0]
  00179952: ldr.w r12,[r3,#0x38]
  00179956: add.w r3,r12,r4
  0017995a: cmp r3,r1
  0017995c: bgt 0x001799bc
  0017995e: add.w r4,lr,#0x1
  00179962: ldr.w r3,[r0,#0x90]
  00179966: sub.w r4,r4,r12
  0017996a: add r3,r4
  0017996c: cmp r3,r1
  0017996e: ble 0x001799bc
  00179970: ldr r3,[r0,#0x7c]
  00179972: movs r1,#0x0
  00179974: subs r4,r3,#0x1
  00179976: cmp r4,r2
  00179978: bgt 0x001799be
  0017997a: ldr.w r0,[r0,#0x8c]
  0017997e: b 0x001799b0
  00179980: ldr.w r12,[r0,#0xc0]
  00179984: sub.w r3,r4,r12
  00179988: cmp r3,r1
  0017998a: bgt 0x001799bc
  0017998c: ldr.w r3,[r0,#0x90]
  00179990: add.w r4,lr,r12
  00179994: add r3,r4
  00179996: adds r3,#0x1
  00179998: cmp r3,r1
  0017999a: ble 0x001799bc
  0017999c: ldr r3,[r0,#0x7c]
  0017999e: mvn.w r1,r12
  001799a2: adds r4,r3,r1
  001799a4: movs r1,#0x0
  001799a6: cmp r4,r2
  001799a8: bgt 0x001799be
  001799aa: ldr.w r0,[r0,#0x8c]
  001799ae: add r3,r12
  001799b0: add r0,r3
  001799b2: adds r0,#0x1
  001799b4: cmp r0,r2
  001799b6: it ge
  001799b8: mov.ge r1,#0x1
  001799ba: b 0x001799be
  001799bc: movs r1,#0x0
  001799be: mov r0,r1
  001799c0: pop {r4,r6,r7,pc}
```
