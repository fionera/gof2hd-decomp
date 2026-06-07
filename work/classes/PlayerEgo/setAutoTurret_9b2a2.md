# PlayerEgo::setAutoTurret
elf 0x9b2a2 body 20
Sig: undefined __stdcall setAutoTurret(bool param_1)

## decompile
```c

/* PlayerEgo::setAutoTurret(bool) */

void PlayerEgo::setAutoTurret(bool param_1)

{
  int in_r1;
  
  *(char *)((int)(uint)param_1 + 0x355) = (char)in_r1;
  if (in_r1 == 0) {
    (*(code *)(DAT_001aba54 + 0x1aba58))(*(undefined4 *)(uint)param_1,2);
    return;
  }
  return;
}

```

## target disasm
```
  000ab2a2: cmp r1,#0x0
  000ab2a4: strb.w r1,[r0,#0x355]
  000ab2a8: beq 0x000ab2ac
  000ab2aa: bx lr
  000ab2ac: ldr r0,[r0,#0x0]
  000ab2ae: movs r1,#0x2
  000ab2b0: b.w 0x001aba48
  001aba48: bx pc
```
