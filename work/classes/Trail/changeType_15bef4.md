# Trail::changeType
elf 0x15bef4 body 110
Sig: undefined __thiscall changeType(Trail * this, int param_1)

## decompile
```c

/* Trail::changeType(int) */

void __thiscall Trail::changeType(Trail *this,int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int iVar4;
  
  switch(param_1) {
  case 1:
  case 7:
  case 0xb:
    puVar2 = (undefined4 *)(DAT_0016bf74 + 0x16bf12);
    iVar4 = DAT_0016bf6c;
    goto LAB_0016bf18;
  case 2:
  case 9:
    uVar3 = *(undefined4 *)(this + 0x14);
    uVar1 = **(undefined4 **)(DAT_0016bf78 + 0x16bf28);
    iVar4 = 0xff00ff;
    break;
  case 3:
  case 6:
  case 10:
    puVar2 = (undefined4 *)(DAT_0016bf84 + 0x16bf1a);
    iVar4 = DAT_0016bf70;
LAB_0016bf18:
    uVar3 = *(undefined4 *)(this + 0x14);
    uVar1 = *(undefined4 *)*puVar2;
    break;
  default:
    uVar3 = *(undefined4 *)(this + 0x14);
    uVar1 = **(undefined4 **)(DAT_0016bf7c + 0x16bf3a);
    iVar4 = -1;
    break;
  case 5:
    uVar3 = *(undefined4 *)(this + 0x14);
    uVar1 = **(undefined4 **)(DAT_0016bf80 + 0x16bf4c);
    iVar4 = 0xff0000;
    break;
  case 8:
    uVar3 = *(undefined4 *)(this + 0x14);
    uVar1 = **(undefined4 **)(DAT_0016bf88 + 0x16bf60);
    iVar4 = DAT_0016bf6c + 0x400000;
  }
  (*(code *)(DAT_001ac5f4 + 0x1ac5f8))(uVar1,uVar3,iVar4);
  return;
}

```

## target disasm
```
  0016bef4: subs r1,#0x1
  0016bef6: cmp r1,#0xa
  0016bef8: bhi 0x0016bf34
  0016befa: tbb [pc,r1]
  0016bf0a: ldr r1,[0x0016bf74]
  0016bf0c: ldr r2,[0x0016bf6c]
  0016bf0e: add r1,pc
  0016bf10: b 0x0016bf18
  0016bf12: ldr r1,[0x0016bf84]
  0016bf14: ldr r2,[0x0016bf70]
  0016bf16: add r1,pc
  0016bf18: ldr r3,[r1,#0x0]
  0016bf1a: ldr r1,[r0,#0x14]
  0016bf1c: ldr r0,[r3,#0x0]
  0016bf1e: b.w 0x001ac5e8
  0016bf22: ldr r1,[0x0016bf78]
  0016bf24: add r1,pc
  0016bf26: ldr r2,[r1,#0x0]
  0016bf28: ldr r1,[r0,#0x14]
  0016bf2a: ldr r0,[r2,#0x0]
  0016bf2c: mov.w r2,#0xff00ff
  0016bf30: b.w 0x001ac5e8
  0016bf34: ldr r1,[0x0016bf7c]
  0016bf36: add r1,pc
  0016bf38: ldr r2,[r1,#0x0]
  0016bf3a: ldr r1,[r0,#0x14]
  0016bf3c: ldr r0,[r2,#0x0]
  0016bf3e: mov.w r2,#0xffffffff
  0016bf42: b.w 0x001ac5e8
  0016bf46: ldr r1,[0x0016bf80]
  0016bf48: add r1,pc
  0016bf4a: ldr r2,[r1,#0x0]
  0016bf4c: ldr r1,[r0,#0x14]
  0016bf4e: ldr r0,[r2,#0x0]
  0016bf50: mov.w r2,#0xff0000
  0016bf54: b.w 0x001ac5e8
  0016bf58: ldr r1,[0x0016bf88]
  0016bf5a: ldr r3,[0x0016bf6c]
  0016bf5c: add r1,pc
  0016bf5e: ldr r2,[r1,#0x0]
  0016bf60: ldr r1,[r0,#0x14]
  0016bf62: ldr r0,[r2,#0x0]
  0016bf64: add.w r2,r3,#0x400000
  0016bf68: b.w 0x001ac5e8
  001ac5e8: bx pc
```
