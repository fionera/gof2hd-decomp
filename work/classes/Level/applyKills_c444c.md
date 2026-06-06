# Level::applyKills
elf 0xc444c body 38
Sig: undefined __thiscall applyKills(Level * this)

## decompile
```c

/* Level::applyKills() */

void __thiscall Level::applyKills(Level *this)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = *(undefined4 **)(DAT_000d4474 + 0xd4458);
  iVar1 = Status::getMission();
  if (iVar1 != 0) {
    Status::addKills((Status *)*puVar2,*(int *)(this + 300));
    *(undefined4 *)(this + 300) = 0;
  }
  return;
}

```
## target disasm
```
  000d444c: push {r4,r5,r7,lr}
  000d444e: add r7,sp,#0x8
  000d4450: mov r4,r0
  000d4452: ldr r0,[0x000d4474]
  000d4454: add r0,pc
  000d4456: ldr r5,[r0,#0x0]
  000d4458: ldr r0,[r5,#0x0]
  000d445a: blx 0x00072850
  000d445e: cbz r0,0x000d4470
  000d4460: ldr.w r1,[r4,#0x12c]
  000d4464: ldr r0,[r5,#0x0]
  000d4466: blx 0x000743ec
  000d446a: movs r0,#0x0
  000d446c: str.w r0,[r4,#0x12c]
  000d4470: pop {r4,r5,r7,pc}
```
