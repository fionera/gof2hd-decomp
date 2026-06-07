# WantedWindow::getWantedAtPosition
elf 0xe1fc0 body 94
Sig: undefined __thiscall getWantedAtPosition(WantedWindow * this, int param_1, int param_2)

## decompile
```c

/* WantedWindow::getWantedAtPosition(int, int) */

uint __thiscall WantedWindow::getWantedAtPosition(WantedWindow *this,int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  
  if (param_1 < *(int *)(this + 0x1c) + (*(int *)(this + 0x24) >> 1)) {
    piVar3 = *(int **)(this + 0x38);
    iVar2 = **(int **)(DAT_000f2020 + 0xf1fe4);
    uVar1 = __aeabi_idiv(((((param_2 - *(int *)(this + 0x20)) - *(int *)(iVar2 + 0xc)) -
                          *(int *)(iVar2 + 0x20)) - *(int *)(iVar2 + 0x5c)) - *(int *)(this + 0x84),
                         *(int *)(iVar2 + 0x70) + *(int *)(iVar2 + 0x34));
    if (*piVar3 - 1U < uVar1) {
      uVar1 = 0xffffffff;
    }
    return uVar1;
  }
  return 0xffffffff;
}

```

## target disasm
```
  000f1fc0: ldr r3,[r0,#0x24]
  000f1fc2: ldr.w r12,[r0,#0x1c]
  000f1fc6: add.w r3,r12,r3, asr #0x1
  000f1fca: cmp r3,r1
  000f1fcc: itt le
  000f1fce: mov.le.w r0,#0xffffffff
  000f1fd2: bx.le lr
  000f1fd4: push {r4,r5,r6,r7,lr}
  000f1fd6: add r7,sp,#0xc
  000f1fd8: push.w r8
  000f1fdc: ldr r1,[0x000f2020]
  000f1fde: ldr r3,[r0,#0x20]
  000f1fe0: add r1,pc
  000f1fe2: ldr.w r8,[r0,#0x38]
  000f1fe6: subs r2,r2,r3
  000f1fe8: ldr.w r12,[r0,#0x84]
  000f1fec: ldr r1,[r1,#0x0]
  000f1fee: ldr r1,[r1,#0x0]
  000f1ff0: ldr r4,[r1,#0xc]
  000f1ff2: ldr r5,[r1,#0x20]
  000f1ff4: subs r2,r2,r4
  000f1ff6: ldr r0,[r1,#0x5c]
  000f1ff8: ldr r6,[r1,#0x34]
  000f1ffa: subs r2,r2,r5
  000f1ffc: ldr r1,[r1,#0x70]
  000f1ffe: subs r0,r2,r0
  000f2000: sub.w r0,r0,r12
  000f2004: add r1,r6
  000f2006: blx 0x0007198c
  000f200a: ldr.w r1,[r8,#0x0]
  000f200e: subs r1,#0x1
  000f2010: cmp r0,r1
  000f2012: it hi
  000f2014: mov.hi.w r0,#0xffffffff
  000f2018: pop.w r8
  000f201c: pop {r4,r5,r6,r7,pc}
```
