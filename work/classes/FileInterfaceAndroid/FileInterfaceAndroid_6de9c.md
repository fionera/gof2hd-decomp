# FileInterfaceAndroid::FileInterfaceAndroid
elf 0x6de9c body 182
Sig: undefined __thiscall FileInterfaceAndroid(FileInterfaceAndroid * this, _jobject * param_1, bool param_2)

## decompile
```c

/* FileInterfaceAndroid::FileInterfaceAndroid(_jobject*, bool) */

FileInterfaceAndroid * __thiscall
FileInterfaceAndroid::FileInterfaceAndroid
          (FileInterfaceAndroid *this,_jobject *param_1,bool param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int *piVar5;
  undefined4 *puVar6;
  
  puVar6 = *(undefined4 **)(DAT_0007df54 + 0x7deb2);
  piVar5 = *(int **)(DAT_0007df58 + 0x7deb4);
  iVar1 = *(int *)(DAT_0007df5c + 0x7deb8);
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(_jobject **)(this + 0x10) = param_1;
  this[0x14] = (FileInterfaceAndroid)param_2;
  *(int *)this = iVar1 + 8;
  piVar2 = (int *)*puVar6;
  *piVar5 = *piVar5 + 1;
  uVar3 = (**(code **)(*piVar2 + 0x7c))();
  if (param_2) {
    piVar2 = *(int **)(DAT_0007df60 + 0x7dee4);
    if (*piVar2 == 0) {
      iVar1 = (**(code **)(*(int *)*puVar6 + 0x84))
                        ((int *)*puVar6,uVar3,DAT_0007df64 + 0x7defa,DAT_0007df68 + 0x7defc);
      *piVar2 = iVar1;
    }
    puVar4 = (undefined4 *)(DAT_0007df6c + 0x7df06);
  }
  else {
    piVar2 = *(int **)(DAT_0007df70 + 0x7df0c);
    if (*piVar2 == 0) {
      iVar1 = (**(code **)(*(int *)*puVar6 + 0x84))
                        ((int *)*puVar6,uVar3,DAT_0007df74 + 0x7df22,DAT_0007df78 + 0x7df24);
      *piVar2 = iVar1;
    }
    puVar4 = (undefined4 *)(DAT_0007df7c + 0x7df2e);
  }
  piVar2 = (int *)*puVar4;
  if (*piVar2 == 0) {
    iVar1 = (**(code **)(*(int *)*puVar6 + 0x84))
                      ((int *)*puVar6,uVar3,DAT_0007df80 + 0x7df44,DAT_0007df84 + 0x7df46);
    *piVar2 = iVar1;
  }
  return this;
}

```

## target disasm
```
  0007de9c: push {r4,r5,r6,r7,lr}
  0007de9e: add r7,sp,#0xc
  0007dea0: push {r8,r9,r11}
  0007dea4: mov r4,r0
  0007dea6: ldr r0,[0x0007df54]
  0007dea8: mov r6,r2
  0007deaa: ldr r2,[0x0007df58]
  0007deac: ldr r3,[0x0007df5c]
  0007deae: add r0,pc
  0007deb0: add r2,pc
  0007deb2: movs r5,#0x0
  0007deb4: add r3,pc
  0007deb6: ldr.w r9,[r0,#0x0]
  0007deba: ldr r2,[r2,#0x0]
  0007debc: ldr r0,[r3,#0x0]
  0007debe: strd r5,r5,[r4,#0x8]
  0007dec2: str r1,[r4,#0x10]
  0007dec4: adds r0,#0x8
  0007dec6: strb r6,[r4,#0x14]
  0007dec8: str r0,[r4,#0x0]
  0007deca: ldr r3,[r2,#0x0]
  0007decc: ldr.w r0,[r9,#0x0]
  0007ded0: adds r3,#0x1
  0007ded2: str r3,[r2,#0x0]
  0007ded4: ldr r2,[r0,#0x0]
  0007ded6: ldr r2,[r2,#0x7c]
  0007ded8: blx r2
  0007deda: mov r8,r0
  0007dedc: cbz r6,0x0007df06
  0007dede: ldr r0,[0x0007df60]
  0007dee0: add r0,pc
  0007dee2: ldr r6,[r0,#0x0]
  0007dee4: ldr r0,[r6,#0x0]
  0007dee6: cbnz r0,0x0007df00
  0007dee8: ldr.w r0,[r9,#0x0]
  0007deec: ldr r1,[r0,#0x0]
  0007deee: ldr r2,[0x0007df64]
  0007def0: ldr r3,[0x0007df68]
  0007def2: ldr.w r5,[r1,#0x84]
  0007def6: add r2,pc
  0007def8: add r3,pc
  0007defa: mov r1,r8
  0007defc: blx r5
  0007defe: str r0,[r6,#0x0]
  0007df00: ldr r0,[0x0007df6c]
  0007df02: add r0,pc
  0007df04: b 0x0007df2c
  0007df06: ldr r0,[0x0007df70]
  0007df08: add r0,pc
  0007df0a: ldr r6,[r0,#0x0]
  0007df0c: ldr r0,[r6,#0x0]
  0007df0e: cbnz r0,0x0007df28
  0007df10: ldr.w r0,[r9,#0x0]
  0007df14: ldr r1,[r0,#0x0]
  0007df16: ldr r2,[0x0007df74]
  0007df18: ldr r3,[0x0007df78]
  0007df1a: ldr.w r5,[r1,#0x84]
  0007df1e: add r2,pc
  0007df20: add r3,pc
  0007df22: mov r1,r8
  0007df24: blx r5
  0007df26: str r0,[r6,#0x0]
  0007df28: ldr r0,[0x0007df7c]
  0007df2a: add r0,pc
  0007df2c: ldr r6,[r0,#0x0]
  0007df2e: ldr r0,[r6,#0x0]
  0007df30: cbnz r0,0x0007df4a
  0007df32: ldr.w r0,[r9,#0x0]
  0007df36: ldr r1,[r0,#0x0]
  0007df38: ldr r2,[0x0007df80]
  0007df3a: ldr r3,[0x0007df84]
  0007df3c: ldr.w r5,[r1,#0x84]
  0007df40: add r2,pc
  0007df42: add r3,pc
  0007df44: mov r1,r8
  0007df46: blx r5
  0007df48: str r0,[r6,#0x0]
  0007df4a: mov r0,r4
  0007df4c: pop.w {r8,r9,r11}
  0007df50: pop {r4,r5,r6,r7,pc}
```
