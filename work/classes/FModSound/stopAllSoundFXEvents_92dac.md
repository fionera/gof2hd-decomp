# FModSound::stopAllSoundFXEvents
elf 0x92dac body 50
Sig: undefined __stdcall stopAllSoundFXEvents(void)

## decompile
```c

/* FModSound::stopAllSoundFXEvents() */

void FModSound::stopAllSoundFXEvents(void)

{
  int in_r0;
  uint uVar1;
  uint uVar2;
  
  if (*(int *)(in_r0 + 0x23fc) != 0) {
    uVar2 = 0;
    while( true ) {
      do {
        uVar1 = uVar2;
        uVar2 = uVar1 + 1;
      } while ((uVar2 & 0x7fffffff) == 1);
      if ((uVar2 & 0x7fffffff) == 4) break;
      (**(code **)(**(int **)(in_r0 + 0x23f0 + uVar1 * 4) + 0x1c))();
    }
  }
  return;
}

```

## target disasm
```
  000a2dac: push {r4,r5,r7,lr}
  000a2dae: add r7,sp,#0x8
  000a2db0: movw r1,#0x23fc
  000a2db4: ldr r1,[r0,r1]
  000a2db6: cbz r1,0x000a2ddc
  000a2db8: movw r1,#0x23f0
  000a2dbc: adds r4,r0,r1
  000a2dbe: movs r0,#0x0
  000a2dc0: b 0x000a2dce
  000a2dc2: ldr.w r0,[r4,r0,lsl #0x2]
  000a2dc6: ldr r1,[r0,#0x0]
  000a2dc8: ldr r1,[r1,#0x1c]
  000a2dca: blx r1
  000a2dcc: mov r0,r5
  000a2dce: adds r5,r0,#0x1
  000a2dd0: bic r1,r5,#0x80000000
  000a2dd4: cmp r1,#0x1
  000a2dd6: beq 0x000a2dcc
  000a2dd8: cmp r1,#0x4
  000a2dda: bne 0x000a2dc2
  000a2ddc: pop {r4,r5,r7,pc}
```
