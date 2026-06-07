# TextureAlphaTestShader::SetInActive
elf 0x89278 body 42
Sig: undefined __thiscall SetInActive(TextureAlphaTestShader * this)

## decompile
```c

/* AbyssEngine::TextureAlphaTestShader::SetInActive() */

void __thiscall AbyssEngine::TextureAlphaTestShader::SetInActive(TextureAlphaTestShader *this)

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
  00099278: push {r4,r5,r6,r7,lr}
  0009927a: add r7,sp,#0xc
  0009927c: push.w r11
  00099280: mov r4,r0
  00099282: movs r5,#0x0
  00099284: b 0x00099298
  00099286: add.w r6,r4,r5, lsl #0x2
  0009928a: ldr r0,[r6,#0x24]
  0009928c: blx 0x00070a44
  00099290: ldr r0,[r6,#0x2c]
  00099292: blx 0x00070a44
  00099296: adds r5,#0x1
  00099298: cmp r5,#0x2
  0009929a: bne 0x00099286
  0009929c: pop.w r11
  000992a0: pop {r4,r5,r6,r7,pc}
```
