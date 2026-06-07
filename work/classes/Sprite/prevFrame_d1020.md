# Sprite::prevFrame
elf 0xd1020 body 10
Sig: undefined __thiscall prevFrame(Sprite * this)

## decompile
```c

/* Sprite::prevFrame() */

void __thiscall Sprite::prevFrame(Sprite *this)

{
  (*(code *)(DAT_001ac084 + 0x1ac088))(this,*(int *)(this + 0x38) + -1);
  return;
}

```

## target disasm
```
  000e1020: ldr r1,[r0,#0x38]
  000e1022: subs r1,#0x1
  000e1024: b.w 0x001ac078
  001ac078: bx pc
```
