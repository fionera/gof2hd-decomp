# Standing::applyDelict
elf 0x11d93c body 72
Sig: undefined __thiscall applyDelict(Standing * this, int param_1, int param_2)

## decompile
```c

/* Standing::applyDelict(int, int) */

void __thiscall Standing::applyDelict(Standing *this,int param_1,int param_2)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar1 = Status::hardCoreMode();
  if (3 < (uint)param_1) {
    return;
  }
  iVar3 = param_2 << (uVar1 & 0xff);
  switch(param_1) {
  case 0:
    iVar3 = -iVar3;
  case 1:
    uVar2 = 0;
    break;
  case 2:
    iVar3 = -iVar3;
  case 3:
    uVar2 = 1;
  }
  (*(code *)(DAT_001ac3d4 + 0x1ac3d8))(this,uVar2,iVar3);
  return;
}

```

## target disasm
```
  0012d93c: push {r4,r5,r6,r7,lr}
  0012d93e: add r7,sp,#0xc
  0012d940: push.w r11
  0012d944: mov r4,r0
  0012d946: ldr r0,[0x0012d988]
  0012d948: mov r6,r2
  0012d94a: mov r5,r1
  0012d94c: add r0,pc
  0012d94e: ldr r0,[r0,#0x0]
  0012d950: ldr r0,[r0,#0x0]
  0012d952: blx 0x0007192c
  0012d956: cmp r5,#0x3
  0012d958: bhi 0x0012d96e
  0012d95a: lsl.w r2,r6,r0
  0012d95e: tbb [pc,r5]
  0012d966: rsbs r2,r2
  0012d968: mov r0,r4
  0012d96a: movs r1,#0x0
  0012d96c: b 0x0012d97a
  0012d96e: pop.w r11
  0012d972: pop {r4,r5,r6,r7,pc}
  0012d974: rsbs r2,r2
  0012d976: mov r0,r4
  0012d978: movs r1,#0x1
  0012d97a: pop.w r11
  0012d97e: pop.w {r4,r5,r6,r7,lr}
  0012d982: b.w 0x001ac3c8
  001ac3c8: bx pc
```
