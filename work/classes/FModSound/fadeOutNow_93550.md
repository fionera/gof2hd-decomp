# FModSound::fadeOutNow
elf 0x93550 body 20
Sig: undefined __thiscall fadeOutNow(FModSound * this)

## decompile
```c

/* FModSound::fadeOutNow() */

void __thiscall FModSound::fadeOutNow(FModSound *this)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)this;
  iVar1 = 0;
  if (iVar2 != -1) {
    iVar1 = *(int *)(this + 4);
  }
  if (iVar2 == -1 || iVar1 == iVar2) {
    return;
  }
  (*(code *)(DAT_001ab814 + 0x1ab818))(this,0,iVar2,DAT_000a3564);
  return;
}

```

## target disasm
```
  000a3550: ldr r2,[r0,#0x0]
  000a3552: adds r1,r2,#0x1
  000a3554: itte ne
  000a3556: ldr.ne r1,[r0,#0x4]
  000a3558: cmp.ne r1,r2
  000a355a: bx.eq lr
  000a355c: ldr r3,[0x000a3564]
  000a355e: movs r1,#0x0
  000a3560: b.w 0x001ab808
```
