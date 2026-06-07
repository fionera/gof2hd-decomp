# ModStation::autosave
elf 0xd7054 body 86
Sig: undefined __stdcall autosave(void)

## decompile
```c

/* ModStation::autosave() */

void ModStation::autosave(void)

{
  int in_r0;
  RecordHandler *this;
  void *pvVar1;
  undefined8 uVar2;
  
  uVar2 = Status::getPlayingTime();
  if ((int)(uint)((int)uVar2 == 0) <= (int)((ulonglong)uVar2 >> 0x20)) {
    this = operator_new(0x2c);
    RecordHandler::RecordHandler(this);
    RecordHandler::recordStoreWrite(this,0);
    RecordHandler::recordStoreWritePreview(this,0);
    pvVar1 = (void *)RecordHandler::~RecordHandler(this);
    operator_delete(pvVar1);
    *(undefined1 *)(in_r0 + 0xb1) = 1;
    if (*(int *)(in_r0 + 0x50) != 0) {
      (*(code *)(DAT_001ac104 + 0x1ac108))();
      return;
    }
  }
  return;
}

```

## target disasm
```
  000e7054: push {r4,r5,r7,lr}
  000e7056: add r7,sp,#0x8
  000e7058: mov r4,r0
  000e705a: ldr r0,[0x000e70b8]
  000e705c: add r0,pc
  000e705e: ldr r0,[r0,#0x0]
  000e7060: ldr r0,[r0,#0x0]
  000e7062: blx 0x00071710
  000e7066: subs r0,#0x1
  000e7068: sbcs r0,r1,#0x0
  000e706c: blt 0x000e70a6
  000e706e: movs r0,#0x2c
  000e7070: blx 0x0006eb24
  000e7074: mov r5,r0
  000e7076: blx 0x0007177c
  000e707a: mov r0,r5
  000e707c: movs r1,#0x0
  000e707e: blx 0x000717a0
  000e7082: mov r0,r5
  000e7084: movs r1,#0x0
  000e7086: blx 0x000717ac
  000e708a: mov r0,r5
  000e708c: blx 0x00071794
  000e7090: blx 0x0006eb48
  000e7094: ldr r0,[r4,#0x50]
  000e7096: movs r1,#0x1
  000e7098: strb.w r1,[r4,#0xb1]
  000e709c: cbz r0,0x000e70a6
  000e709e: pop.w {r4,r5,r7,lr}
  000e70a2: b.w 0x001ac0f8
  000e70a6: pop {r4,r5,r7,pc}
  001ac0f8: bx pc
```
