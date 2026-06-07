# AESoundRessource::setMusicVolume
elf 0x7fd70 body 46
Sig: undefined __thiscall setMusicVolume(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::setMusicVolume(int) */

void __thiscall AbyssEngine::AESoundRessource::setMusicVolume(AESoundRessource *this,int param_1)

{
  int *piVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 4); uVar2 = uVar2 + 1) {
    piVar1 = *(int **)(*(int *)(this + 8) + uVar2 * 4);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 0x30))(piVar1,param_1);
    }
  }
  return;
}

```

## target disasm
```
  0008fd70: push {r4,r5,r6,r7,lr}
  0008fd72: add r7,sp,#0xc
  0008fd74: push.w r11
  0008fd78: mov r4,r1
  0008fd7a: mov r5,r0
  0008fd7c: movs r6,#0x0
  0008fd7e: b 0x0008fd92
  0008fd80: ldr r0,[r5,#0x8]
  0008fd82: ldr.w r0,[r0,r6,lsl #0x2]
  0008fd86: cbz r0,0x0008fd90
  0008fd88: ldr r1,[r0,#0x0]
  0008fd8a: ldr r2,[r1,#0x30]
  0008fd8c: mov r1,r4
  0008fd8e: blx r2
  0008fd90: adds r6,#0x1
  0008fd92: ldr r0,[r5,#0x4]
  0008fd94: cmp r6,r0
  0008fd96: bcc 0x0008fd80
  0008fd98: pop.w r11
  0008fd9c: pop {r4,r5,r6,r7,pc}
```
