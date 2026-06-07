# AESoundRessource::freeAllRessources
elf 0x7fb0c body 48
Sig: undefined __thiscall freeAllRessources(AESoundRessource * this)

## decompile
```c

/* AbyssEngine::AESoundRessource::freeAllRessources() */

void __thiscall AbyssEngine::AESoundRessource::freeAllRessources(AESoundRessource *this)

{
  void *pvVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 4); uVar2 = uVar2 + 1) {
    pvVar1 = *(void **)(*(int *)(this + 8) + uVar2 * 4);
    if (pvVar1 != (void *)0x0) {
      operator_delete(pvVar1);
      *(undefined4 *)(*(int *)(this + 8) + uVar2 * 4) = 0;
    }
  }
  return;
}

```

## target disasm
```
  0008fb0c: push {r4,r5,r6,r7,lr}
  0008fb0e: add r7,sp,#0xc
  0008fb10: push.w r11
  0008fb14: mov r4,r0
  0008fb16: movs r5,#0x0
  0008fb18: movs r6,#0x0
  0008fb1a: b 0x0008fb30
  0008fb1c: ldr r0,[r4,#0x8]
  0008fb1e: ldr.w r0,[r0,r6,lsl #0x2]
  0008fb22: cbz r0,0x0008fb2e
  0008fb24: blx 0x0006eb48
  0008fb28: ldr r0,[r4,#0x8]
  0008fb2a: str.w r5,[r0,r6,lsl #0x2]
  0008fb2e: adds r6,#0x1
  0008fb30: ldr r0,[r4,#0x4]
  0008fb32: cmp r6,r0
  0008fb34: bcc 0x0008fb1c
  0008fb36: pop.w r11
  0008fb3a: pop {r4,r5,r6,r7,pc}
```
