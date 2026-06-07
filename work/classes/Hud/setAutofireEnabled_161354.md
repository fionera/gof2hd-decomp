# Hud::setAutofireEnabled
elf 0x161354 body 6
Sig: undefined __thiscall setAutofireEnabled(Hud * this, bool param_1)

## decompile
```c

/* Hud::setAutofireEnabled(bool) */

void __thiscall Hud::setAutofireEnabled(Hud *this,bool param_1)

{
  this[0x4a0] = (Hud)param_1;
  return;
}

```

## target disasm
```
  00171354: strb.w r1,[r0,#0x4a0]
  00171358: bx lr
```
