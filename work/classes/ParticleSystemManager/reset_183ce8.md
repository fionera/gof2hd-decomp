# ParticleSystemManager::reset
elf 0x183ce8 body 60
Sig: undefined __thiscall reset(ParticleSystemManager * this)

## decompile
```c

/* ParticleSystemManager::reset() */

void __thiscall ParticleSystemManager::reset(ParticleSystemManager *this)

{
  int *piVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 0x18); uVar2 = uVar2 + 1) {
    piVar1 = *(int **)(*(int *)(this + 0x1c) + uVar2 * 4);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))();
    }
  }
  for (uVar2 = 0; uVar2 < *(uint *)(this + 0x3c); uVar2 = uVar2 + 1) {
    piVar1 = *(int **)(*(int *)(this + 0x40) + uVar2 * 4);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))();
    }
  }
  return;
}

```

## target disasm
```
  00193ce8: push {r4,r5,r7,lr}
  00193cea: add r7,sp,#0x8
  00193cec: mov r4,r0
  00193cee: movs r5,#0x0
  00193cf0: b 0x00193d02
  00193cf2: ldr r0,[r4,#0x1c]
  00193cf4: ldr.w r0,[r0,r5,lsl #0x2]
  00193cf8: cbz r0,0x00193d00
  00193cfa: ldr r1,[r0,#0x0]
  00193cfc: ldr r1,[r1,#0x8]
  00193cfe: blx r1
  00193d00: adds r5,#0x1
  00193d02: ldr r0,[r4,#0x18]
  00193d04: cmp r5,r0
  00193d06: bcc 0x00193cf2
  00193d08: movs r5,#0x0
  00193d0a: b 0x00193d1c
  00193d0c: ldr r0,[r4,#0x40]
  00193d0e: ldr.w r0,[r0,r5,lsl #0x2]
  00193d12: cbz r0,0x00193d1a
  00193d14: ldr r1,[r0,#0x0]
  00193d16: ldr r1,[r1,#0x8]
  00193d18: blx r1
  00193d1a: adds r5,#0x1
  00193d1c: ldr r0,[r4,#0x3c]
  00193d1e: cmp r5,r0
  00193d20: bcc 0x00193d0c
  00193d22: pop {r4,r5,r7,pc}
```
