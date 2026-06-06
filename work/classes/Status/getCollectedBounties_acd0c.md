# Status::getCollectedBounties
elf 0xacd0c body 14
Sig: undefined __thiscall getCollectedBounties(Status * this, int param_1)

## decompile
```c

/* Status::getCollectedBounties(int) */

undefined4 __thiscall Status::getCollectedBounties(Status *this,int param_1)

{
  undefined4 uVar1;
  
  if (param_1 < 4) {
    uVar1 = *(undefined4 *)(this + param_1 * 4 + 4);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}

```
## target disasm
```
  000bcd0c: cmp r1,#0x3
  000bcd0e: itee gt
  000bcd10: mov.gt r0,#0x0
  000bcd12: add.le.w r0,r0,r1, lsl #0x2
  000bcd16: ldr.le r0,[r0,#0x4]
  000bcd18: bx lr
```
