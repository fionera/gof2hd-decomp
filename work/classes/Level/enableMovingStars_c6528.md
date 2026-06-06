# Level::enableMovingStars
elf 0xc6528 body 20
Sig: undefined __thiscall enableMovingStars(Level * this, bool param_1)

## decompile
```c

/* Level::enableMovingStars(bool) */

void __thiscall Level::enableMovingStars(Level *this,bool param_1)

{
  if (*(int *)(this + 100) < 0) {
    return;
  }
  (*(code *)(DAT_001ac044 + 0x1ac048))(*(undefined4 *)(this + 0x88),*(int *)(this + 100),param_1);
  return;
}

```
## target disasm
```
  000d6528: mov r2,r1
  000d652a: ldr r1,[r0,#0x64]
  000d652c: cmp r1,#0x0
  000d652e: blt 0x000d6538
  000d6530: ldr.w r0,[r0,#0x88]
  000d6534: b.w 0x001ac038
  000d6538: bx lr
  001ac038: bx pc
```
