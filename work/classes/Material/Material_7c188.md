# Material::Material
elf 0x7c188 body 104
Sig: undefined __thiscall Material(Material * this)

## decompile
```c

/* AbyssEngine::Material::Material() */

Material * __thiscall AbyssEngine::Material::Material(Material *this)

{
  int iVar1;
  
  *(undefined4 *)(this + 0x20) = 0;
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x2c));
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x38));
  Array<AbyssEngine::Mesh*>::Array((Array<AbyssEngine::Mesh*> *)(this + 0x44));
  Array<unsigned_int>::Array((Array<unsigned_int> *)(this + 0x50));
  Array<AbyssEngine::AEMath::Matrix>::Array((Array<AbyssEngine::AEMath::Matrix> *)(this + 0x5c));
  *(undefined4 *)(this + 0x68) = DAT_0008c21c;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  for (iVar1 = 0; iVar1 != 8; iVar1 = iVar1 + 1) {
    *(undefined4 *)(this + iVar1 * 4) = 0xffffffff;
  }
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  return this;
}

```

## target disasm
```
  0008c188: push {r4,r5,r6,r7,lr}
  0008c18a: add r7,sp,#0xc
  0008c18c: push {r8,r9,r11}
  0008c190: mov r6,r0
  0008c192: add.w r8,r6,#0x2c
  0008c196: movs r0,#0x0
  0008c198: str r0,[r6,#0x20]
  0008c19a: mov r0,r8
  0008c19c: blx 0x0006f8e0
  0008c1a0: add.w r9,r6,#0x38
  0008c1a4: mov r0,r9
  0008c1a6: blx 0x0006f8e0
  0008c1aa: add.w r4,r6,#0x44
  0008c1ae: mov r0,r4
  0008c1b0: blx 0x0006f724
  0008c1b4: add.w r5,r6,#0x50
  0008c1b8: mov r0,r5
  0008c1ba: blx 0x0006f8ec
  0008c1be: add.w r0,r6,#0x5c
  0008c1c2: blx 0x0006f8e0
  0008c1c6: ldr r1,[0x0008c21c]
  0008c1c8: movs r0,#0x0
  0008c1ca: strd r1,r0,[r6,#0x68]
  0008c1ce: mov.w r1,#0xffffffff
  0008c1d2: str r0,[r6,#0x70]
  0008c1d4: b 0x0008c1dc
  0008c1d6: str.w r1,[r6,r0,lsl #0x2]
  0008c1da: adds r0,#0x1
  0008c1dc: cmp r0,#0x8
  0008c1de: bne 0x0008c1d6
  0008c1e0: movs r0,#0x0
  0008c1e2: str r0,[r6,#0x0]
  0008c1e4: strd r0,r0,[r6,#0x24]
  0008c1e8: mov r0,r6
  0008c1ea: pop.w {r8,r9,r11}
  0008c1ee: pop {r4,r5,r6,r7,pc}
```
