# FModSound::getEventPauseLength
elf 0x93a14 body 86
Sig: undefined __stdcall getEventPauseLength(int param_1)

## decompile
```c

/* FModSound::getEventPauseLength(int) */

void FModSound::getEventPauseLength(int param_1)

{
  int in_r1;
  int iVar1;
  char *pcVar2;
  int *piVar3;
  
  piVar3 = *(int **)(DAT_000a3a6c + 0xa3a1e);
  iVar1 = *piVar3;
  if ((((*(int *)(param_1 + 0x2404) != 0) && (*(int *)(param_1 + 0x23fc) != 0)) &&
      (*(char *)(param_1 + 0x11) != '\0')) &&
     (pcVar2 = *(char **)(param_1 + in_r1 * 4 + 0x18), pcVar2 != (char *)0x0)) {
    FMOD::Event::getProperty(pcVar2,(void *)**(undefined4 **)(DAT_000a3a70 + 0xa3a4c),true);
  }
  if (*piVar3 == iVar1) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(0);
}

```

## target disasm
```
  000a3a14: push {r2,r3,r4,r6,r7,lr}
  000a3a16: add r7,sp,#0x10
  000a3a18: ldr r2,[0x000a3a6c]
  000a3a1a: add r2,pc
  000a3a1c: ldr r4,[r2,#0x0]
  000a3a1e: ldr r2,[r4,#0x0]
  000a3a20: str r2,[sp,#0x4]
  000a3a22: movw r2,#0x2404
  000a3a26: ldr r2,[r0,r2]
  000a3a28: cbz r2,0x000a3a58
  000a3a2a: movw r2,#0x23fc
  000a3a2e: ldr r2,[r0,r2]
  000a3a30: cbz r2,0x000a3a58
  000a3a32: ldrb r2,[r0,#0x11]
  000a3a34: cbz r2,0x000a3a58
  000a3a36: add.w r0,r0,r1, lsl #0x2
  000a3a3a: ldr r3,[r0,#0x18]
  000a3a3c: movs r0,#0x0
  000a3a3e: cbz r3,0x000a3a5a
  000a3a40: ldr r1,[0x000a3a70]
  000a3a42: mov r2,sp
  000a3a44: str r0,[sp,#0x0]
  000a3a46: mov r0,r3
  000a3a48: add r1,pc
  000a3a4a: movs r3,#0x1
  000a3a4c: ldr r1,[r1,#0x0]
  000a3a4e: ldr r1,[r1,#0x0]
  000a3a50: blx 0x00071584
  000a3a54: ldr r0,[sp,#0x0]
  000a3a56: b 0x000a3a5a
  000a3a58: movs r0,#0x0
  000a3a5a: ldr r1,[sp,#0x4]
  000a3a5c: ldr r2,[r4,#0x0]
  000a3a5e: subs r1,r2,r1
  000a3a60: itt eq
  000a3a62: add.eq sp,#0x8
  000a3a64: pop.eq {r4,r6,r7,pc}
  000a3a66: blx 0x0006e824
```
