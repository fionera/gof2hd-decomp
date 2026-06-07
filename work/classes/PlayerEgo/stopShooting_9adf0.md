# PlayerEgo::stopShooting
elf 0x9adf0 body 68
Sig: undefined __stdcall stopShooting(int param_1)

## decompile
```c

/* PlayerEgo::stopShooting(int) */

void PlayerEgo::stopShooting(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  int in_r1;
  
  if (*(char *)(param_1 + 0x1a0) == '\0') {
    iVar2 = isDead((PlayerEgo *)param_1);
    if (iVar2 != 0) {
      return;
    }
    uVar1 = *(undefined4 *)param_1;
    if (in_r1 == 1) {
      (*(code *)(DAT_001aba64 + 0x1aba68))(uVar1,1,*(undefined4 *)(param_1 + 0x10c));
      return;
    }
  }
  else {
    uVar1 = *(undefined4 *)param_1;
  }
  (*(code *)(DAT_001aba54 + 0x1aba58))(uVar1);
  return;
}

```

## target disasm
```
  000aadf0: push {r4,r5,r7,lr}
  000aadf2: add r7,sp,#0x8
  000aadf4: mov r4,r0
  000aadf6: ldrb.w r0,[r0,#0x1a0]
  000aadfa: mov r5,r1
  000aadfc: cbz r0,0x000aae0a
  000aadfe: ldr r0,[r4,#0x0]
  000aae00: movs r1,#0x2
  000aae02: pop.w {r4,r5,r7,lr}
  000aae06: b.w 0x001aba48
  000aae0a: mov r0,r4
  000aae0c: blx 0x000724b4
  000aae10: cbz r0,0x000aae14
  000aae12: pop {r4,r5,r7,pc}
  000aae14: ldr r0,[r4,#0x0]
  000aae16: cmp r5,#0x1
  000aae18: bne 0x000aae28
  000aae1a: ldr.w r2,[r4,#0x10c]
  000aae1e: movs r1,#0x1
  000aae20: pop.w {r4,r5,r7,lr}
  000aae24: b.w 0x001aba58
  000aae28: mov r1,r5
  000aae2a: pop.w {r4,r5,r7,lr}
  000aae2e: b.w 0x001aba48
  001aba58: bx pc
```
