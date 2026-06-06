# Status::loadWanted
elf 0xace18 body 40
Sig: undefined __thiscall loadWanted(Status * this)

## decompile
```c

/* Status::loadWanted() */

void __thiscall Status::loadWanted(Status *this)

{
  FileRead *this_00;
  undefined4 uVar1;
  
  this_00 = operator_new(1);
  FileRead::FileRead(this_00);
  uVar1 = FileRead::loadWanted();
  *(undefined4 *)this = uVar1;
  FileRead::~FileRead(this_00);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```
## target disasm
```
  000bce18: push {r4,r5,r7,lr}
  000bce1a: add r7,sp,#0x8
  000bce1c: mov r5,r0
  000bce1e: movs r0,#0x1
  000bce20: blx 0x0006eb24
  000bce24: mov r4,r0
  000bce26: blx 0x00072124
  000bce2a: mov r0,r4
  000bce2c: blx 0x000739fc
  000bce30: str r0,[r5,#0x0]
  000bce32: mov r0,r4
  000bce34: blx 0x0007213c
  000bce38: pop.w {r4,r5,r7,lr}
  000bce3c: b.w 0x001ab098
```
