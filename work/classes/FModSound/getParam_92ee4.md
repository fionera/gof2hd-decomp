# FModSound::getParam
elf 0x92ee4 body 56
Sig: undefined __thiscall getParam(FModSound * this, char * param_1, int param_2)

## decompile
```c

/* FModSound::getParam(char const*, int) */

void __thiscall FModSound::getParam(FModSound *this,char *param_1,int param_2)

{
  FModSound *pFVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = *(int **)(DAT_000a2f1c + 0xa2eee);
  iVar2 = *piVar3;
  if ((*(int *)(this + 0x23fc) != 0) &&
     (pFVar1 = this + param_2 * 4 + 0x18, this = (FModSound *)0x0, *(char **)pFVar1 != (char *)0x0))
  {
    FMOD::Event::getParameter(*(char **)pFVar1,(EventParameter **)param_1);
    this = (FModSound *)param_2;
  }
  if (*piVar3 != iVar2) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(this);
  }
  return;
}

```

## target disasm
```
  000a2ee4: push {r2,r3,r4,r6,r7,lr}
  000a2ee6: add r7,sp,#0x10
  000a2ee8: ldr r3,[0x000a2f1c]
  000a2eea: add r3,pc
  000a2eec: ldr r4,[r3,#0x0]
  000a2eee: ldr r3,[r4,#0x0]
  000a2ef0: str r3,[sp,#0x4]
  000a2ef2: movw r3,#0x23fc
  000a2ef6: ldr r3,[r0,r3]
  000a2ef8: cbz r3,0x000a2f0c
  000a2efa: add.w r0,r0,r2, lsl #0x2
  000a2efe: ldr r0,[r0,#0x18]
  000a2f00: cbz r0,0x000a2f0c
  000a2f02: mov r2,sp
  000a2f04: blx 0x000714f4
  000a2f08: ldr r0,[sp,#0x0]
  000a2f0a: b 0x000a2f0c
  000a2f0c: ldr r1,[sp,#0x4]
  000a2f0e: ldr r2,[r4,#0x0]
  000a2f10: subs r1,r2,r1
  000a2f12: itt eq
  000a2f14: add.eq sp,#0x8
  000a2f16: pop.eq {r4,r6,r7,pc}
  000a2f18: blx 0x0006e824
```
