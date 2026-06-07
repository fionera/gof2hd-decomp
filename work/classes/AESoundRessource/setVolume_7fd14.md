# AESoundRessource::setVolume
elf 0x7fd14 body 46
Sig: undefined __thiscall setVolume(AESoundRessource * this, int param_1)

## decompile
```c

/* AbyssEngine::AESoundRessource::setVolume(int) */

void __thiscall AbyssEngine::AESoundRessource::setVolume(AESoundRessource *this,int param_1)

{
  int *piVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 4); uVar2 = uVar2 + 1) {
    piVar1 = *(int **)(*(int *)(this + 8) + uVar2 * 4);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 0x28))(piVar1,param_1);
    }
  }
  return;
}

```

## target disasm
```
  0008fd14: push {r4,r5,r6,r7,lr}
  0008fd16: add r7,sp,#0xc
  0008fd18: push.w r11
  0008fd1c: mov r4,r1
  0008fd1e: mov r5,r0
  0008fd20: movs r6,#0x0
  0008fd22: b 0x0008fd36
  0008fd24: ldr r0,[r5,#0x8]
  0008fd26: ldr.w r0,[r0,r6,lsl #0x2]
  0008fd2a: cbz r0,0x0008fd34
  0008fd2c: ldr r1,[r0,#0x0]
  0008fd2e: ldr r2,[r1,#0x28]
  0008fd30: mov r1,r4
  0008fd32: blx r2
  0008fd34: adds r6,#0x1
  0008fd36: ldr r0,[r5,#0x4]
  0008fd38: cmp r6,r0
  0008fd3a: bcc 0x0008fd24
  0008fd3c: pop.w r11
  0008fd40: pop {r4,r5,r6,r7,pc}
```
