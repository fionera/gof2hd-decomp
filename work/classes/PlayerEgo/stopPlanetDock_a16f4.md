# PlayerEgo::stopPlanetDock
elf 0xa16f4 body 44
Sig: undefined __stdcall stopPlanetDock(void)

## decompile
```c

/* PlayerEgo::stopPlanetDock() */

void PlayerEgo::stopPlanetDock(void)

{
  int in_r0;
  
  TargetFollowCamera::setLookAtCam(SUB41(*(undefined4 *)(in_r0 + 0x88),0));
  *(undefined1 *)(in_r0 + 0x1ee) = 0;
  *(undefined1 *)(in_r0 + 0x144) = 1;
  stopBoost();
  *(undefined4 *)(in_r0 + 0xb8) = 0x40000000;
  return;
}

```

## target disasm
```
  000b16f4: push {r4,r5,r7,lr}
  000b16f6: add r7,sp,#0x8
  000b16f8: mov r4,r0
  000b16fa: ldr.w r0,[r0,#0x88]
  000b16fe: movs r1,#0x0
  000b1700: movs r5,#0x0
  000b1702: blx 0x00072670
  000b1706: movs r0,#0x1
  000b1708: strb.w r5,[r4,#0x1ee]
  000b170c: strb.w r0,[r4,#0x144]
  000b1710: mov r0,r4
  000b1712: blx 0x00072cac
  000b1716: mov.w r0,#0x40000000
  000b171a: str.w r0,[r4,#0xb8]
  000b171e: pop {r4,r5,r7,pc}
```
