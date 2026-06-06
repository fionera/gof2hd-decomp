# Level::enableFog
elf 0xc651c body 12
Sig: undefined __thiscall enableFog(Level * this, bool param_1)

## decompile
```c

/* Level::enableFog(bool) */

void __thiscall Level::enableFog(Level *this,bool param_1)

{
  (*(code *)(DAT_001ac044 + 0x1ac048))
            (*(undefined4 *)(this + 0x7c),*(undefined4 *)(this + 0x284),param_1);
  return;
}

```
## target disasm
```
  000d651c: mov r2,r1
  000d651e: ldr.w r1,[r0,#0x284]
  000d6522: ldr r0,[r0,#0x7c]
  000d6524: b.w 0x001ac038
```
