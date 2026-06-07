# PlayerEgo::dockToPlanet
elf 0xa1684 body 106
Sig: undefined __stdcall dockToPlanet(void)

## decompile
```c

/* PlayerEgo::dockToPlanet() */

void PlayerEgo::dockToPlanet(void)

{
  int iVar1;
  int in_r0;
  int *piVar2;
  float fVar3;
  
  TargetFollowCamera::setLookAtCam(SUB41(*(undefined4 *)(in_r0 + 0x88),0));
  *(undefined1 *)(in_r0 + 0x13c) = 1;
  *(undefined4 *)(in_r0 + 0x138) = 0;
  *(undefined4 *)(in_r0 + 0xb8) = 0x41000000;
  *(undefined4 *)(in_r0 + 0xcc) = 10000;
  *(undefined4 *)(in_r0 + 0xd0) = 0;
  *(undefined1 *)(in_r0 + 0x144) = 0;
  fVar3 = (float)resetGunDelay();
  iVar1 = DAT_000b16f0;
  *(undefined1 *)(in_r0 + 0x38) = 0;
  *(undefined1 *)(in_r0 + 0x1ee) = 1;
  *(undefined4 *)(in_r0 + 0xb8) = 0x41000000;
  piVar2 = *(int **)(iVar1 + 0xb16d0);
  *(undefined4 *)(in_r0 + 0x1f0) = 0;
  FModSound::play(*piVar2,(Vector *)&DAT_00000005,(Vector *)0x0,fVar3);
  return;
}

```

## target disasm
```
  000b1684: push {r4,r5,r6,r7,lr}
  000b1686: add r7,sp,#0xc
  000b1688: push.w r8
  000b168c: sub sp,#0x8
  000b168e: mov r4,r0
  000b1690: ldr.w r0,[r0,#0x88]
  000b1694: movs r1,#0x1
  000b1696: mov.w r8,#0x1
  000b169a: blx 0x00072670
  000b169e: movs r6,#0x0
  000b16a0: mov.w r5,#0x41000000
  000b16a4: movw r0,#0x2710
  000b16a8: strb.w r8,[r4,#0x13c]
  000b16ac: str.w r6,[r4,#0x138]
  000b16b0: str.w r5,[r4,#0xb8]
  000b16b4: strd r0,r6,[r4,#0xcc]
  000b16b8: mov r0,r4
  000b16ba: strb.w r6,[r4,#0x144]
  000b16be: blx 0x00072ca0
  000b16c2: ldr r0,[0x000b16f0]
  000b16c4: movs r1,#0x5
  000b16c6: strb.w r6,[r4,#0x38]
  000b16ca: movs r2,#0x0
  000b16cc: add r0,pc
  000b16ce: strb.w r8,[r4,#0x1ee]
  000b16d2: str.w r5,[r4,#0xb8]
  000b16d6: movs r3,#0x0
  000b16d8: ldr r0,[r0,#0x0]
  000b16da: str.w r6,[r4,#0x1f0]
  000b16de: str r6,[sp,#0x0]
  000b16e0: ldr r0,[r0,#0x0]
  000b16e2: blx 0x00071548
  000b16e6: add sp,#0x8
  000b16e8: pop.w r8
  000b16ec: pop {r4,r5,r6,r7,pc}
```
