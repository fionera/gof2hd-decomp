# NoTexShader::SetInActive
elf 0x906b0 body 6
Sig: undefined __thiscall SetInActive(NoTexShader * this)

## decompile
```c

/* AbyssEngine::NoTexShader::SetInActive() */

void __thiscall AbyssEngine::NoTexShader::SetInActive(NoTexShader *this)

{
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x20));
  return;
}

```

## target disasm
```
  000a06b0: ldr r0,[r0,#0x20]
  000a06b2: b.w 0x001ab778
```
