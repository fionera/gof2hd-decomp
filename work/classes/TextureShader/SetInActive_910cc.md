# TextureShader::SetInActive
elf 0x910cc body 42
Sig: undefined __thiscall SetInActive(TextureShader * this)

## decompile
```c

/* AbyssEngine::TextureShader::SetInActive() */

void __thiscall AbyssEngine::TextureShader::SetInActive(TextureShader *this)

{
  int iVar1;
  
  for (iVar1 = 0; iVar1 != 2; iVar1 = iVar1 + 1) {
    glDisableVertexAttribArray(*(undefined4 *)(this + iVar1 * 4 + 0x24));
    glDisableVertexAttribArray(*(undefined4 *)(this + iVar1 * 4 + 0x2c));
  }
  return;
}

```

## target disasm
```
  000a10cc: push {r4,r5,r6,r7,lr}
  000a10ce: add r7,sp,#0xc
  000a10d0: push.w r11
  000a10d4: mov r4,r0
  000a10d6: movs r5,#0x0
  000a10d8: b 0x000a10ec
  000a10da: add.w r6,r4,r5, lsl #0x2
  000a10de: ldr r0,[r6,#0x24]
  000a10e0: blx 0x00070a44
  000a10e4: ldr r0,[r6,#0x2c]
  000a10e6: blx 0x00070a44
  000a10ea: adds r5,#0x1
  000a10ec: cmp r5,#0x2
  000a10ee: bne 0x000a10da
  000a10f0: pop.w r11
  000a10f4: pop {r4,r5,r6,r7,pc}
```
