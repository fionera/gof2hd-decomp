# DrawFBOShader::DrawFBOShader
elf 0x88490 body 36
Sig: undefined __thiscall DrawFBOShader(DrawFBOShader * this)

## decompile
```c

/* AbyssEngine::DrawFBOShader::DrawFBOShader() */

void __thiscall AbyssEngine::DrawFBOShader::DrawFBOShader(DrawFBOShader *this)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  piVar1 = (int *)ShaderBaseStruct::ShaderBaseStruct((ShaderBaseStruct *)this);
  puVar2 = *(undefined4 **)(DAT_000984b4 + 0x984a2);
  puVar3 = *(undefined4 **)(DAT_000984bc + 0x984a6);
  *piVar1 = *(int *)(DAT_000984b8 + 0x984a4) + 8;
  *puVar3 = *puVar2;
  return;
}

```

## target disasm
```
  00098490: push {r7,lr}
  00098492: mov r7,sp
  00098494: blx 0x00070930
  00098498: ldr r1,[0x000984b4]
  0009849a: ldr r2,[0x000984b8]
  0009849c: ldr r3,[0x000984bc]
  0009849e: add r1,pc
  000984a0: add r2,pc
  000984a2: add r3,pc
  000984a4: ldr r1,[r1,#0x0]
  000984a6: ldr r2,[r2,#0x0]
  000984a8: ldr r3,[r3,#0x0]
  000984aa: adds r2,#0x8
  000984ac: str r2,[r0,#0x0]
  000984ae: ldr r1,[r1,#0x0]
  000984b0: str r1,[r3,#0x0]
  000984b2: pop {r7,pc}
```
