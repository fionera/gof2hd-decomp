# PlayerTurret::handleTurret
elf 0x157782 body 56
Sig: undefined __stdcall handleTurret(int param_1)

## decompile
```c

/* PlayerTurret::handleTurret(int) */

void PlayerTurret::handleTurret(int param_1)

{
  int in_r1;
  
  *(int *)(param_1 + 0x130) = *(int *)(param_1 + 0x130) + in_r1;
  pickEnemy((PlayerTurret *)param_1);
  if ((*(int *)(param_1 + 0x14c) != 0) && (*(char *)(*(int *)(param_1 + 0x14c) + 0x5e) == '\0')) {
    (*(code *)(DAT_001ac5c4 + 0x1ac5c8))(param_1);
    return;
  }
  return;
}

```

## target disasm
```
  00167782: push {r4,r5,r7,lr}
  00167784: add r7,sp,#0x8
  00167786: mov r5,r0
  00167788: ldr.w r0,[r0,#0x130]
  0016778c: mov r4,r1
  0016778e: add r0,r1
  00167790: str.w r0,[r5,#0x130]
  00167794: mov r0,r5
  00167796: blx 0x000776b0
  0016779a: ldr.w r0,[r5,#0x14c]
  0016779e: cbz r0,0x001677a6
  001677a0: ldrb.w r0,[r0,#0x5e]
  001677a4: cbz r0,0x001677a8
  001677a6: pop {r4,r5,r7,pc}
  001677a8: ldrd r3,r2,[r5,#0x144]
  001677ac: mov r0,r5
  001677ae: mov r1,r4
  001677b0: pop.w {r4,r5,r7,lr}
  001677b4: b.w 0x001ac5b8
  001ac5b8: bx pc
```
