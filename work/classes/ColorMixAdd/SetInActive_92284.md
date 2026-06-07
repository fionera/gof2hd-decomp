# ColorMixAdd::SetInActive
elf 0x92284 body 30
Sig: undefined __thiscall SetInActive(ColorMixAdd * this)

## decompile
```c

/* AbyssEngine::ColorMixAdd::SetInActive() */

void __thiscall AbyssEngine::ColorMixAdd::SetInActive(ColorMixAdd *this)

{
  int iVar1;
  
  for (iVar1 = 2; iVar1 != 0; iVar1 = iVar1 + -1) {
    glDisableVertexAttribArray(*(undefined4 *)(this + 0x20));
    glDisableVertexAttribArray(*(undefined4 *)(this + 0x24));
  }
  return;
}

```

## target disasm
```
  000a2284: push {r4,r5,r7,lr}
  000a2286: add r7,sp,#0x8
  000a2288: mov r4,r0
  000a228a: movs r5,#0x2
  000a228c: b 0x000a229c
  000a228e: ldr r0,[r4,#0x20]
  000a2290: blx 0x00070a44
  000a2294: ldr r0,[r4,#0x24]
  000a2296: blx 0x00070a44
  000a229a: subs r5,#0x1
  000a229c: cmp r5,#0x0
  000a229e: bne 0x000a228e
  000a22a0: pop {r4,r5,r7,pc}
```
