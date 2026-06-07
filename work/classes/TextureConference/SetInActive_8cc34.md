# TextureConference::SetInActive
elf 0x8cc34 body 22
Sig: undefined __thiscall SetInActive(TextureConference * this)

## decompile
```c

/* AbyssEngine::TextureConference::SetInActive() */

void __thiscall AbyssEngine::TextureConference::SetInActive(TextureConference *this)

{
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x20));
  (*(code *)(DAT_001ab784 + 0x1ab788))(*(undefined4 *)(this + 0x24));
  return;
}

```

## target disasm
```
  0009cc34: push {r4,r6,r7,lr}
  0009cc36: add r7,sp,#0x8
  0009cc38: mov r4,r0
  0009cc3a: ldr r0,[r0,#0x20]
  0009cc3c: blx 0x00070a44
  0009cc40: ldr r0,[r4,#0x24]
  0009cc42: pop.w {r4,r6,r7,lr}
  0009cc46: b.w 0x001ab778
```
