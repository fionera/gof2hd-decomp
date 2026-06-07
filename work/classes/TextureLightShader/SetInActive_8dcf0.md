# TextureLightShader::SetInActive
elf 0x8dcf0 body 30
Sig: undefined __thiscall SetInActive(TextureLightShader * this)

## decompile
```c

/* AbyssEngine::TextureLightShader::SetInActive() */

void __thiscall AbyssEngine::TextureLightShader::SetInActive(TextureLightShader *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_0009dd10 + 0x9dcfe);
  (*pcVar1)(*(undefined4 *)(this + 0x20));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  (*pcVar1)(*(undefined4 *)(this + 0x28));
  return;
}

```

## target disasm
```
  0009dcf0: push {r4,r5,r7,lr}
  0009dcf2: add r7,sp,#0x8
  0009dcf4: ldr r1,[0x0009dd10]
  0009dcf6: mov r4,r0
  0009dcf8: ldr r0,[r0,#0x20]
  0009dcfa: add r1,pc
  0009dcfc: ldr r5,[r1,#0x0]
  0009dcfe: blx r5
  0009dd00: ldr r0,[r4,#0x24]
  0009dd02: blx r5
  0009dd04: ldr r0,[r4,#0x28]
  0009dd06: mov r1,r5
  0009dd08: pop.w {r4,r5,r7,lr}
  0009dd0c: bx r1
```
