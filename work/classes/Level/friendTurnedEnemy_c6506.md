# Level::friendTurnedEnemy
elf 0xc6506 body 24
Sig: undefined __stdcall friendTurnedEnemy(int param_1)

## decompile
```c

/* Level::friendTurnedEnemy(int) */

void Level::friendTurnedEnemy(int param_1)

{
  if (*(char *)(param_1 + 0x188) == '\0') {
    *(undefined1 *)(param_1 + 0x188) = 1;
    (*(code *)(DAT_001ac024 + 0x1ac028))(param_1,0);
    return;
  }
  return;
}

```
## target disasm
```
  000d6506: mov r2,r1
  000d6508: ldrb.w r1,[r0,#0x188]
  000d650c: cbz r1,0x000d6510
  000d650e: bx lr
  000d6510: movs r1,#0x1
  000d6512: strb.w r1,[r0,#0x188]
  000d6516: movs r1,#0x0
  000d6518: b.w 0x001ac018
  001ac018: bx pc
```
