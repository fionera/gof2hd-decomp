# BloomShader::~BloomShader
elf 0x8947c body 16
Sig: undefined __thiscall ~BloomShader(BloomShader * this)

## decompile
```c

/* AbyssEngine::BloomShader::~BloomShader() */

void __thiscall AbyssEngine::BloomShader::~BloomShader(BloomShader *this)

{
  ~BloomShader(this);
  (*(code *)(DAT_001ab0a4 + 0x1ab0a8))();
  return;
}

```

## target disasm
```
  0009947c: push {r7,lr}
  0009947e: mov r7,sp
  00099480: blx 0x00070bdc
  00099484: pop.w {r7,lr}
  00099488: b.w 0x001ab098
```
