# FileInterfaceAndroid::FileInterfaceAndroid
elf 0x6de18 body 70
Sig: undefined __thiscall FileInterfaceAndroid(FileInterfaceAndroid * this, zip_file * param_1, bool param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* FileInterfaceAndroid::FileInterfaceAndroid(zip_file*, bool, int, int, int) */

FileInterfaceAndroid * __thiscall
FileInterfaceAndroid::FileInterfaceAndroid
          (FileInterfaceAndroid *this,zip_file *param_1,bool param_2,int param_3,int param_4,
          int param_5)

{
  undefined4 *puVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  
  puVar1 = (undefined4 *)(DAT_0007de60 + 0x7de2c);
  piVar3 = (int *)(DAT_0007de64 + 0x7de30);
  *(undefined4 *)(this + 8) = 0;
  *(zip_file **)(this + 0xc) = param_1;
  piVar2 = (int *)*puVar1;
  iVar4 = *piVar3;
  *(undefined4 *)(this + 0x10) = 0;
  this[0x14] = (FileInterfaceAndroid)0x0;
  *(int *)this = iVar4 + 8;
  *piVar2 = *piVar2 + 1;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  Seek(this,param_3);
  this[0x24] = (FileInterfaceAndroid)param_2;
  return this;
}

```

## target disasm
```
  0007de18: push {r4,r5,r6,r7,lr}
  0007de1a: add r7,sp,#0xc
  0007de1c: push.w r11
  0007de20: mov r5,r0
  0007de22: ldr r0,[0x0007de60]
  0007de24: mov r6,r2
  0007de26: ldr r2,[0x0007de64]
  0007de28: add r0,pc
  0007de2a: movs r4,#0x0
  0007de2c: add r2,pc
  0007de2e: strd r4,r1,[r5,#0x8]
  0007de32: ldr r0,[r0,#0x0]
  0007de34: ldr r2,[r2,#0x0]
  0007de36: str r4,[r5,#0x10]
  0007de38: strb r4,[r5,#0x14]
  0007de3a: add.w r1,r2,#0x8
  0007de3e: str r1,[r5,#0x0]
  0007de40: ldr r1,[r0,#0x0]
  0007de42: adds r1,#0x1
  0007de44: str r1,[r0,#0x0]
  0007de46: mov r0,r5
  0007de48: mov r1,r3
  0007de4a: str r4,[r5,#0x28]
  0007de4c: str r4,[r5,#0x1c]
  0007de4e: blx 0x0006ef08
  0007de52: strb.w r6,[r5,#0x24]
  0007de56: mov r0,r5
  0007de58: pop.w r11
  0007de5c: pop {r4,r5,r6,r7,pc}
```
