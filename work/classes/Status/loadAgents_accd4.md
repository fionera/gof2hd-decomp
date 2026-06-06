# Status::loadAgents
elf 0xaccd4 body 40
Sig: undefined __thiscall loadAgents(Status * this)

## decompile
```c

/* Status::loadAgents() */

void __thiscall Status::loadAgents(Status *this)

{
  FileRead *this_00;
  undefined4 uVar1;
  
  this_00 = operator_new(1);
  FileRead::FileRead(this_00);
  uVar1 = FileRead::loadAgents();
  *(undefined4 *)(this + 0x20) = uVar1;
  FileRead::~FileRead(this_00);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```
## target disasm
```
  000bccd4: push {r4,r5,r7,lr}
  000bccd6: add r7,sp,#0x8
  000bccd8: mov r5,r0
  000bccda: movs r0,#0x1
  000bccdc: blx 0x0006eb24
  000bcce0: mov r4,r0
  000bcce2: blx 0x00072124
  000bcce6: mov r0,r4
  000bcce8: blx 0x000739f0
  000bccec: str r0,[r5,#0x20]
  000bccee: mov r0,r4
  000bccf0: blx 0x0007213c
  000bccf4: pop.w {r4,r5,r7,lr}
  000bccf8: b.w 0x001ab098
```
