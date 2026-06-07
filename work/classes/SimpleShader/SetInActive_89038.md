# SimpleShader::SetInActive
elf 0x89038 body 6
Sig: undefined __thiscall SetInActive(SimpleShader * this)

## decompile
```c

/* AbyssEngine::SimpleShader::SetInActive() */

void __thiscall AbyssEngine::SimpleShader::SetInActive(SimpleShader *this)

{
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x20));
  return;
}

```

## target disasm
```
  00099038: ldr r0,[r0,#0x20]
  0009903a: b.w 0x001ab778
```
