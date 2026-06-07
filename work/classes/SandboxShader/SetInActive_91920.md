# SandboxShader::SetInActive
elf 0x91920 body 38
Sig: undefined __thiscall SetInActive(SandboxShader * this)

## decompile
```c

/* AbyssEngine::SandboxShader::SetInActive() */

void __thiscall AbyssEngine::SandboxShader::SetInActive(SandboxShader *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_000a1948 + 0xa192e);
  (*pcVar1)(*(undefined4 *)(this + 0x20));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  (*pcVar1)(*(undefined4 *)(this + 0x28));
  (*pcVar1)(*(undefined4 *)(this + 0x2c));
  (*pcVar1)(*(undefined4 *)(this + 0x30));
  return;
}

```

## target disasm
```
  000a1920: push {r4,r5,r7,lr}
  000a1922: add r7,sp,#0x8
  000a1924: ldr r1,[0x000a1948]
  000a1926: mov r4,r0
  000a1928: ldr r0,[r0,#0x20]
  000a192a: add r1,pc
  000a192c: ldr r5,[r1,#0x0]
  000a192e: blx r5
  000a1930: ldr r0,[r4,#0x24]
  000a1932: blx r5
  000a1934: ldr r0,[r4,#0x28]
  000a1936: blx r5
  000a1938: ldr r0,[r4,#0x2c]
  000a193a: blx r5
  000a193c: ldr r0,[r4,#0x30]
  000a193e: mov r1,r5
  000a1940: pop.w {r4,r5,r7,lr}
  000a1944: bx r1
```
