# PlayerTurret::handleSentryGun
elf 0x15774c body 54
Sig: undefined __stdcall handleSentryGun(int param_1)

## decompile
```c

/* PlayerTurret::handleSentryGun(int) */

void PlayerTurret::handleSentryGun(int param_1)

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
  0016774c: push {r4,r5,r7,lr}
  0016774e: add r7,sp,#0x8
  00167750: mov r5,r0
  00167752: ldr.w r0,[r0,#0x130]
  00167756: mov r4,r1
  00167758: add r0,r1
  0016775a: str.w r0,[r5,#0x130]
  0016775e: mov r0,r5
  00167760: blx 0x000776b0
  00167764: ldr.w r0,[r5,#0x14c]
  00167768: cbz r0,0x00167770
  0016776a: ldrb.w r0,[r0,#0x5e]
  0016776e: cbz r0,0x00167772
  00167770: pop {r4,r5,r7,pc}
  00167772: ldr r2,[r5,#0x8]
  00167774: mov r0,r5
  00167776: mov r1,r4
  00167778: mov r3,r2
  0016777a: pop.w {r4,r5,r7,lr}
  0016777e: b.w 0x001ac5b8
```
