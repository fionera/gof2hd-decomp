# PlayerJunk::reset
elf 0x15e77c body 38
Sig: undefined __thiscall reset(PlayerJunk * this)

## decompile
```c

/* PlayerJunk::reset() */

void __thiscall PlayerJunk::reset(PlayerJunk *this)

{
  KIPlayer::reset((KIPlayer *)this);
  *(undefined4 *)(this + 0x88) = 0;
  (*(code *)(DAT_001abe14 + 0x1abe18))(this,1);
  return;
}

```

## target disasm
```
  0016e77c: push {r4,r6,r7,lr}
  0016e77e: add r7,sp,#0x8
  0016e780: mov r4,r0
  0016e782: blx 0x00074518
  0016e786: movs r0,#0x0
  0016e788: movs r1,#0x1
  0016e78a: str.w r0,[r4,#0x88]
  0016e78e: mov r0,r4
  0016e790: pop.w {r4,r6,r7,lr}
  0016e794: b.w 0x001abe08
  001abe08: bx pc
  001abe0c: ldr r12,[0x1abe14]
  001abe10: add pc,r12,pc
```
