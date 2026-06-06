# Item::canBeInstalledMultipleTimes
elf 0xe0100 body 10
Sig: undefined __thiscall canBeInstalledMultipleTimes(Item * this)

## decompile
```c

/* Item::canBeInstalledMultipleTimes() */

undefined1 __thiscall Item::canBeInstalledMultipleTimes(Item *this)

{
  return *(undefined1 *)(DAT_000f010c + 0xf0108 + *(int *)(this + 8));
}

```
## target disasm
```
  000f0100: ldr r1,[0x000f010c]
  000f0102: ldr r0,[r0,#0x8]
  000f0104: add r1,pc
  000f0106: ldrb r0,[r1,r0]
  000f0108: bx lr
```
