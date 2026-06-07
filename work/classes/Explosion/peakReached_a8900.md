# Explosion::peakReached
elf 0xa8900 body 22
Sig: undefined __thiscall peakReached(Explosion * this)

## decompile
```c

/* Explosion::peakReached() */

bool __thiscall Explosion::peakReached(Explosion *this)

{
  int iVar1;
  
  iVar1 = *(int *)(this + 0x1c);
  return (int)(-(uint)(0x8fc < *(uint *)(this + 0x18)) - iVar1) < 0 !=
         (SBORROW4(0,iVar1) != SBORROW4(-iVar1,(uint)(0x8fc < *(uint *)(this + 0x18))));
}

```

## target disasm
```
  000b8900: ldrd r1,r2,[r0,#0x18]
  000b8904: movw r3,#0x8fc
  000b8908: movs r0,#0x0
  000b890a: subs r1,r3,r1
  000b890c: sbcs.w r1,r0,r2
  000b8910: it lt
  000b8912: mov.lt r0,#0x1
  000b8914: bx lr
```
