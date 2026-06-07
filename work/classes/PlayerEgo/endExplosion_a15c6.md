# PlayerEgo::endExplosion
elf 0xa15c6 body 14
Sig: undefined __thiscall endExplosion(PlayerEgo * this)

## decompile
```c

/* PlayerEgo::endExplosion() */

void __thiscall PlayerEgo::endExplosion(PlayerEgo *this)

{
  if (this[3].field_20 == 0) {
    return;
  }
  (*(code *)(DAT_001abbf4 + 0x1abbf8))();
  return;
}

```

## target disasm
```
  000b15c6: ldr.w r0,[r0,#0x8c]
  000b15ca: cmp r0,#0x0
  000b15cc: it ne
  000b15ce: b.ne.w 0x001abbe8
  000b15d2: bx lr
```
