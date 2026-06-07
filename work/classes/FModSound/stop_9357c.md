# FModSound::stop
elf 0x9357c body 14
Sig: undefined __thiscall stop(FModSound * this, Event * param_1)

## decompile
```c

/* FModSound::stop(FMOD::Event*) */

FModSound * __thiscall FModSound::stop(FModSound *this,Event *param_1)

{
  FModSound *pFVar1;
  
  if (param_1 == (Event *)0x0) {
    return this;
  }
  pFVar1 = (FModSound *)(*(code *)(DAT_001ab824 + 0x1ab828))(param_1,0);
  return pFVar1;
}

```

## target disasm
```
  000a357c: cbz r1,0x000a3586
  000a357e: mov r0,r1
  000a3580: movs r1,#0x0
  000a3582: b.w 0x001ab818
  000a3586: bx lr
  001ab818: bx pc
```
