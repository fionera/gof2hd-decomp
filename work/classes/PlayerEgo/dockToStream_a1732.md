# PlayerEgo::dockToStream
elf 0xa1732 body 64
Sig: undefined __stdcall dockToStream(bool param_1)

## decompile
```c

/* PlayerEgo::dockToStream(bool) */

void PlayerEgo::dockToStream(bool param_1)

{
  uint uVar1;
  int in_r1;
  
  uVar1 = (uint)param_1;
  if (in_r1 == 0) {
    *(undefined4 *)(uVar1 + 0xb8) = 0x40000000;
    setPosition(uVar1,*(undefined4 *)(uVar1 + 0xe0),*(undefined4 *)(uVar1 + 0xe4),
                *(undefined4 *)(uVar1 + 0xe8));
    *(undefined1 *)(uVar1 + 0x24) = 0;
    *(undefined1 *)(uVar1 + 0x145) = 0;
    *(undefined2 *)(uVar1 + 0x1ec) = 0;
    (*(code *)(&UNK_001abc28 + DAT_001abc24))(uVar1,0);
    return;
  }
  *(undefined2 *)(uVar1 + 0x1ec) = 0x100;
  return;
}

```

## target disasm
```
  000b1732: push {r4,r6,r7,lr}
  000b1734: add r7,sp,#0x8
  000b1736: mov r4,r0
  000b1738: cbz r1,0x000b1744
  000b173a: mov.w r0,#0x100
  000b173e: strh.w r0,[r4,#0x1ec]
  000b1742: pop {r4,r6,r7,pc}
  000b1744: add.w r3,r4,#0xe0
  000b1748: mov.w r0,#0x40000000
  000b174c: ldmia r3,{r1,r2,r3}
  000b174e: str.w r0,[r4,#0xb8]
  000b1752: mov r0,r4
  000b1754: blx 0x00072cb8
  000b1758: movs r0,#0x0
  000b175a: movs r1,#0x0
  000b175c: strb.w r0,[r4,#0x24]
  000b1760: strb.w r0,[r4,#0x145]
  000b1764: strh.w r0,[r4,#0x1ec]
  000b1768: mov r0,r4
  000b176a: pop.w {r4,r6,r7,lr}
  000b176e: b.w 0x001abc18
```
