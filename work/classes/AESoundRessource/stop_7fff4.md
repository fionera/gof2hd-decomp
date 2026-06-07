# AESoundRessource::stop
elf 0x7fff4 body 34
Sig: undefined __thiscall stop(AESoundRessource * this)

## decompile
```c

/* AbyssEngine::AESoundRessource::stop() */

void __thiscall AbyssEngine::AESoundRessource::stop(AESoundRessource *this)

{
  int *piVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 4); uVar2 = uVar2 + 1) {
    piVar1 = *(int **)(*(int *)(this + 8) + uVar2 * 4);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 0x1c))();
    }
  }
  return;
}

```

## target disasm
```
  0008fff4: push {r4,r5,r7,lr}
  0008fff6: add r7,sp,#0x8
  0008fff8: mov r4,r0
  0008fffa: movs r5,#0x0
  0008fffc: b 0x0009000e
  0008fffe: ldr r0,[r4,#0x8]
  00090000: ldr.w r0,[r0,r5,lsl #0x2]
  00090004: cbz r0,0x0009000c
  00090006: ldr r1,[r0,#0x0]
  00090008: ldr r1,[r1,#0x1c]
  0009000a: blx r1
  0009000c: adds r5,#0x1
  0009000e: ldr r0,[r4,#0x4]
  00090010: cmp r5,r0
  00090012: bcc 0x0008fffe
  00090014: pop {r4,r5,r7,pc}
```
