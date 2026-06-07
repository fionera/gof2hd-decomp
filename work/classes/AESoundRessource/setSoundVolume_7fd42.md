# AESoundRessource::setSoundVolume
elf 0x7fd42 body 46
Sig: undefined __thiscall setSoundVolume(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::setSoundVolume(int) */

void __thiscall AbyssEngine::AESoundRessource::setSoundVolume(AESoundRessource *this,int param_1)

{
  int *piVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 4); uVar2 = uVar2 + 1) {
    piVar1 = *(int **)(*(int *)(this + 8) + uVar2 * 4);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 0x2c))(piVar1,param_1);
    }
  }
  return;
}

```

## target disasm
```
  0008fd42: push {r4,r5,r6,r7,lr}
  0008fd44: add r7,sp,#0xc
  0008fd46: push.w r11
  0008fd4a: mov r4,r1
  0008fd4c: mov r5,r0
  0008fd4e: movs r6,#0x0
  0008fd50: b 0x0008fd64
  0008fd52: ldr r0,[r5,#0x8]
  0008fd54: ldr.w r0,[r0,r6,lsl #0x2]
  0008fd58: cbz r0,0x0008fd62
  0008fd5a: ldr r1,[r0,#0x0]
  0008fd5c: ldr r2,[r1,#0x2c]
  0008fd5e: mov r1,r4
  0008fd60: blx r2
  0008fd62: adds r6,#0x1
  0008fd64: ldr r0,[r5,#0x4]
  0008fd66: cmp r6,r0
  0008fd68: bcc 0x0008fd52
  0008fd6a: pop.w r11
  0008fd6e: pop {r4,r5,r6,r7,pc}
```
