# BloomShader::SetInActive
elf 0x897f4 body 38
Sig: undefined __thiscall SetInActive(BloomShader * this)

## decompile
```c

/* AbyssEngine::BloomShader::SetInActive() */

void __thiscall AbyssEngine::BloomShader::SetInActive(BloomShader *this)

{
  code *pcVar1;
  
  pcVar1 = *(code **)(DAT_0009981c + 0x99804);
  (*pcVar1)(*(undefined4 *)(this + 0x8c));
  (*pcVar1)(*(undefined4 *)(this + 0x94));
  (*pcVar1)(*(undefined4 *)(this + 0x2c));
  (*pcVar1)(*(undefined4 *)(this + 0x24));
  return;
}

```

## target disasm
```
  000997f4: push {r4,r5,r7,lr}
  000997f6: add r7,sp,#0x8
  000997f8: ldr r1,[0x0009981c]
  000997fa: mov r4,r0
  000997fc: ldr.w r0,[r0,#0x8c]
  00099800: add r1,pc
  00099802: ldr r5,[r1,#0x0]
  00099804: blx r5
  00099806: ldr.w r0,[r4,#0x94]
  0009980a: blx r5
  0009980c: ldr r0,[r4,#0x2c]
  0009980e: blx r5
  00099810: ldr r0,[r4,#0x24]
  00099812: mov r1,r5
  00099814: pop.w {r4,r5,r7,lr}
  00099818: bx r1
```
