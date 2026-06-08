# String::Compare
elf 0x72f80 body 64
Sig: undefined __thiscall Compare(String * this, String * param_1)

## decompile
```c

/* AbyssEngine::String::Compare(AbyssEngine::String const&) */

int __thiscall AbyssEngine::String::Compare(String *this,String *param_1)

{
  short sVar1;
  bool bVar2;
  short sVar3;
  short *psVar4;
  short *psVar5;
  
  if (*(int *)(param_1 + 8) == *(int *)(this + 8)) {
    psVar4 = *(short **)(param_1 + 4);
    psVar5 = *(short **)(this + 4);
    while( true ) {
      sVar3 = *psVar5;
      sVar1 = *psVar4;
      if (sVar3 == 0) break;
      if (sVar1 != sVar3) {
        bVar2 = false;
        goto LAB_00082fa8;
      }
      psVar5 = psVar5 + 1;
      psVar4 = psVar4 + 1;
    }
    bVar2 = true;
    sVar3 = 0;
LAB_00082fa8:
    if (sVar1 == 0) {
      if (sVar3 != 0) {
        sVar3 = 1;
      }
    }
    else {
      sVar3 = sVar3 - sVar1;
      if (bVar2) {
        sVar3 = -1;
      }
    }
  }
  else {
    sVar3 = 0xff;
  }
  return (int)(char)sVar3;
}

```

## target disasm
```
  00082f80: ldr r2,[r0,#0x8]
  00082f82: ldr r3,[r1,#0x8]
  00082f84: cmp r3,r2
  00082f86: bne 0x00082fa0
  00082f88: ldr r2,[r1,#0x4]
  00082f8a: ldr r3,[r0,#0x4]
  00082f8c: b 0x00082f92
  00082f8e: adds r3,#0x2
  00082f90: adds r2,#0x2
  00082f92: ldrh r0,[r3,#0x0]
  00082f94: ldrh r1,[r2,#0x0]
  00082f96: cbz r0,0x00082fa4
  00082f98: cmp r1,r0
  00082f9a: beq 0x00082f8e
  00082f9c: movs r2,#0x0
  00082f9e: b 0x00082fa8
  00082fa0: movs r0,#0xff
  00082fa2: b 0x00082fbc
  00082fa4: movs r2,#0x1
  00082fa6: movs r0,#0x0
  00082fa8: cbz r1,0x00082fb6
  00082faa: subs r0,r0,r1
  00082fac: cmp r2,#0x0
  00082fae: it ne
  00082fb0: mov.ne.w r0,#0xffffffff
  00082fb4: b 0x00082fbc
  00082fb6: cmp r0,#0x0
  00082fb8: it ne
  00082fba: mov.ne r0,#0x1
  00082fbc: sxtb r0,r0
  00082fbe: bx lr
```
