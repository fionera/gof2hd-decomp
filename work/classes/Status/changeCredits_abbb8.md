# Status::changeCredits
elf 0xabbb8 body 30
Sig: undefined __stdcall changeCredits(int param_1)

## decompile
```c

/* Status::changeCredits(int) */

void Status::changeCredits(int param_1)

{
  int in_r1;
  uint uVar1;
  int iVar2;
  
  iVar2 = in_r1;
  if (in_r1 < 0) {
    iVar2 = -in_r1;
  }
  if (iVar2 <= DAT_000bbbd8) {
    uVar1 = in_r1 + *(int *)(param_1 + 0x1b0);
    *(uint *)(param_1 + 0x1b0) = uVar1 & ~((int)uVar1 >> 0x1f);
  }
  return;
}

```
## target disasm
```
  000bbbb8: ldr r2,[0x000bbbd8]
  000bbbba: mov r3,r1
  000bbbbc: cmp r1,#0x0
  000bbbbe: it mi
  000bbbc0: rsb.mi r3,r1
  000bbbc2: cmp r3,r2
  000bbbc4: itttt le
  000bbbc6: ldr.le.w r2,[r0,#0x1b0]
  000bbbca: add.le r1,r2
  000bbbcc: bic.le.w r1,r1,r1, asr #0x1f
  000bbbd0: str.w.le r1,[r0,#0x1b0]
  000bbbd4: bx lr
```
