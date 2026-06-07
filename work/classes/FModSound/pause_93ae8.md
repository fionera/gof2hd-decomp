# FModSound::pause
elf 0x93ae8 body 24
Sig: int __thiscall pause(FModSound * this)

## decompile
```c

/* FModSound::pause(int) */

int __thiscall FModSound::pause(FModSound *this)

{
  FModSound *pFVar1;
  int iVar2;
  int in_r1;
  
  if (*(int *)(this + 0x23fc) != 0) {
    pFVar1 = this + in_r1 * 4 + 0x18;
    this = (FModSound *)0x0;
    if (*(int *)pFVar1 != 0) {
      iVar2 = (*(code *)(DAT_001ab834 + 0x1ab838))(*(int *)pFVar1,1);
      return iVar2;
    }
  }
  return (int)this;
}

```

## target disasm
```
  000a3ae8: movw r2,#0x23fc
  000a3aec: ldr r2,[r0,r2]
  000a3aee: cbz r2,0x000a3afe
  000a3af0: add.w r0,r0,r1, lsl #0x2
  000a3af4: ldr r0,[r0,#0x18]
  000a3af6: cbz r0,0x000a3afe
  000a3af8: movs r1,#0x1
  000a3afa: b.w 0x001ab828
  000a3afe: bx lr
```
