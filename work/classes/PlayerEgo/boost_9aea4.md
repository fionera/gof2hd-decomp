# PlayerEgo::boost
elf 0x9aea4 body 88
Sig: undefined __stdcall boost(void)

## decompile
```c

/* PlayerEgo::boost() */

void PlayerEgo::boost(void)

{
  int iVar1;
  int in_r0;
  int *piVar2;
  uint in_fpscr;
  float fVar3;
  
  iVar1 = DAT_000aaefc;
  if ((((*(char *)(in_r0 + 0x13c) == '\0') && (*(char *)(in_r0 + 0x146) != '\0')) &&
      (*(int *)(in_r0 + 0x194) == 0)) && (-1 < *(int *)(in_r0 + 0x138))) {
    fVar3 = (float)VectorSignedToFloat(*(undefined4 *)(in_r0 + 200),(byte)(in_fpscr >> 0x16) & 3);
    *(undefined4 *)(in_r0 + 0x138) = 0;
    *(undefined1 *)(in_r0 + 0x13c) = 1;
    piVar2 = *(int **)(iVar1 + 0xaaee0);
    *(float *)(in_r0 + 0xb8) = fVar3;
    FModSound::play(*piVar2,*(Vector **)(in_r0 + 0xd4),(Vector *)0x0,fVar3);
    return;
  }
  return;
}

```

## target disasm
```
  000aaea4: ldrb.w r1,[r0,#0x13c]
  000aaea8: cbz r1,0x000aaeac
  000aaeaa: bx lr
  000aaeac: ldrb.w r1,[r0,#0x146]
  000aaeb0: cmp r1,#0x0
  000aaeb2: beq 0x000aaeaa
  000aaeb4: ldr.w r1,[r0,#0x194]
  000aaeb8: cmp r1,#0x0
  000aaeba: bne 0x000aaeaa
  000aaebc: ldr.w r1,[r0,#0x138]
  000aaec0: cmp r1,#0x0
  000aaec2: blt 0x000aaeaa
  000aaec4: push {r5,r6,r7,lr}
  000aaec6: add r7,sp,#0x8
  000aaec8: vldr.32 s0,[r0,#0xc8]
  000aaecc: mov.w r12,#0x0
  000aaed0: movs r3,#0x1
  000aaed2: vcvt.f32.s32 s0,s0
  000aaed6: ldr r2,[0x000aaefc]
  000aaed8: str.w r12,[r0,#0x138]
  000aaedc: add r2,pc
  000aaede: ldr.w r1,[r0,#0xd4]
  000aaee2: strb.w r3,[r0,#0x13c]
  000aaee6: movs r3,#0x0
  000aaee8: ldr r2,[r2,#0x0]
  000aaeea: vstr.32 s0,[r0,#0xb8]
  000aaeee: ldr r0,[r2,#0x0]
  000aaef0: movs r2,#0x0
  000aaef2: str.w r12,[sp,#0x0]
  000aaef6: blx 0x00071548
  000aaefa: pop {r2,r3,r7,pc}
```
