# AESoundRessource::supend
elf 0x80018 body 34
Sig: undefined __thiscall supend(AESoundRessource * this)

## decompile
```c

/* AbyssEngine::AESoundRessource::supend() */

void __thiscall AbyssEngine::AESoundRessource::supend(AESoundRessource *this)

{
  int *piVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 4); uVar2 = uVar2 + 1) {
    piVar1 = *(int **)(*(int *)(this + 8) + uVar2 * 4);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 4))();
    }
  }
  return;
}

```

## target disasm
```
  00090018: push {r4,r5,r7,lr}
  0009001a: add r7,sp,#0x8
  0009001c: mov r4,r0
  0009001e: movs r5,#0x0
  00090020: b 0x00090032
  00090022: ldr r0,[r4,#0x8]
  00090024: ldr.w r0,[r0,r5,lsl #0x2]
  00090028: cbz r0,0x00090030
  0009002a: ldr r1,[r0,#0x0]
  0009002c: ldr r1,[r1,#0x4]
  0009002e: blx r1
  00090030: adds r5,#0x1
  00090032: ldr r0,[r4,#0x4]
  00090034: cmp r5,r0
  00090036: bcc 0x00090022
  00090038: pop {r4,r5,r7,pc}
```
