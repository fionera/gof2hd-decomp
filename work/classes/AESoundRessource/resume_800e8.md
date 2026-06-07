# AESoundRessource::resume
elf 0x800e8 body 34
Sig: undefined __thiscall resume(AESoundRessource * this)

## decompile
```c

/* AbyssEngine::AESoundRessource::resume() */

void __thiscall AbyssEngine::AESoundRessource::resume(AESoundRessource *this)

{
  int *piVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 4); uVar2 = uVar2 + 1) {
    piVar1 = *(int **)(*(int *)(this + 8) + uVar2 * 4);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 0x18))();
    }
  }
  return;
}

```

## target disasm
```
  000900e8: push {r4,r5,r7,lr}
  000900ea: add r7,sp,#0x8
  000900ec: mov r4,r0
  000900ee: movs r5,#0x0
  000900f0: b 0x00090102
  000900f2: ldr r0,[r4,#0x8]
  000900f4: ldr.w r0,[r0,r5,lsl #0x2]
  000900f8: cbz r0,0x00090100
  000900fa: ldr r1,[r0,#0x0]
  000900fc: ldr r1,[r1,#0x18]
  000900fe: blx r1
  00090100: adds r5,#0x1
  00090102: ldr r0,[r4,#0x4]
  00090104: cmp r5,r0
  00090106: bcc 0x000900f2
  00090108: pop {r4,r5,r7,pc}
```
