# TextureVtxColorShader::SetInActive
elf 0x8bf18 body 48
Sig: undefined __thiscall SetInActive(TextureVtxColorShader * this)

## decompile
```c

/* AbyssEngine::TextureVtxColorShader::SetInActive() */

void __thiscall AbyssEngine::TextureVtxColorShader::SetInActive(TextureVtxColorShader *this)

{
  int iVar1;
  code *pcVar2;
  
  pcVar2 = *(code **)(DAT_0009bf48 + 0x9bf2a);
  for (iVar1 = 0; iVar1 != 2; iVar1 = iVar1 + 1) {
    (*pcVar2)(*(undefined4 *)(this + iVar1 * 4 + 0x24));
    (*pcVar2)(*(undefined4 *)(this + iVar1 * 4 + 0x2c));
    (*pcVar2)(*(undefined4 *)(this + iVar1 * 4 + 0x34));
  }
  return;
}

```

## target disasm
```
  0009bf18: push {r4,r5,r6,r7,lr}
  0009bf1a: add r7,sp,#0xc
  0009bf1c: push.w r8
  0009bf20: mov r8,r0
  0009bf22: ldr r0,[0x0009bf48]
  0009bf24: movs r5,#0x0
  0009bf26: add r0,pc
  0009bf28: ldr r6,[r0,#0x0]
  0009bf2a: b 0x0009bf3e
  0009bf2c: add.w r4,r8,r5, lsl #0x2
  0009bf30: ldr r0,[r4,#0x24]
  0009bf32: blx r6
  0009bf34: ldr r0,[r4,#0x2c]
  0009bf36: blx r6
  0009bf38: ldr r0,[r4,#0x34]
  0009bf3a: blx r6
  0009bf3c: adds r5,#0x1
  0009bf3e: cmp r5,#0x2
  0009bf40: bne 0x0009bf2c
  0009bf42: pop.w r8
  0009bf46: pop {r4,r5,r6,r7,pc}
```
