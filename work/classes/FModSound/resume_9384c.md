# FModSound::resume
elf 0x9384c body 38
Sig: undefined __thiscall resume(FModSound * this, Event * param_1)

## decompile
```c

/* FModSound::resume(FMOD::Event*) */

bool __thiscall FModSound::resume(FModSound *this,Event *param_1)

{
  bool bVar1;
  int iVar2;
  
  bVar1 = false;
  if ((param_1 != (Event *)0x0) && (*(int *)(this + 0x23fc) != 0)) {
    iVar2 = FMOD::Event::setPaused(SUB41(param_1,0));
    bVar1 = iVar2 == 0;
  }
  return bVar1;
}

```

## target disasm
```
  000a384c: mov r2,r0
  000a384e: movs r0,#0x0
  000a3850: cbz r1,0x000a3870
  000a3852: movw r3,#0x23fc
  000a3856: ldr r2,[r2,r3]
  000a3858: cbz r2,0x000a3870
  000a385a: push {r7,lr}
  000a385c: mov r7,sp
  000a385e: mov r0,r1
  000a3860: movs r1,#0x0
  000a3862: blx 0x000715c0
  000a3866: clz r0,r0
  000a386a: lsrs r0,r0,#0x5
  000a386c: pop.w {r7,lr}
  000a3870: bx lr
```
