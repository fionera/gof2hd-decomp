# Status::changeRating
elf 0xac698 body 36
Sig: undefined __thiscall changeRating(Status * this, int param_1)

## decompile
```c

/* Status::changeRating(int) */

void __thiscall Status::changeRating(Status *this,int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = param_1 + *(int *)(this + 0x1b4);
  *(int *)(this + 0x1b4) = iVar1;
  if (iVar1 < 0xb) {
    if (-0xb < iVar1) {
      return;
    }
    uVar2 = 0xfffffff6;
  }
  else {
    uVar2 = 10;
  }
  *(undefined4 *)(this + 0x1b4) = uVar2;
  return;
}

```
## target disasm
```
  000bc698: ldr.w r2,[r0,#0x1b4]
  000bc69c: add r1,r2
  000bc69e: str.w r1,[r0,#0x1b4]
  000bc6a2: cmp r1,#0xa
  000bc6a4: ble 0x000bc6aa
  000bc6a6: movs r1,#0xa
  000bc6a8: b 0x000bc6b6
  000bc6aa: cmn r1,#0xb
  000bc6ae: it gt
  000bc6b0: bx.gt lr
  000bc6b2: mvn r1,#0x9
  000bc6b6: str.w r1,[r0,#0x1b4]
  000bc6ba: bx lr
```
