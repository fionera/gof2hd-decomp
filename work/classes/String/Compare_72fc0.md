# String::Compare
elf 0x72fc0 body 68
Sig: undefined __thiscall Compare(String * this, char * param_1)

## decompile
```c

/* AbyssEngine::String::Compare(char const*) */

uint __thiscall AbyssEngine::String::Compare(String *this,char *param_1)

{
  bool bVar1;
  char cVar2;
  ushort uVar3;
  ushort *puVar4;
  ushort uVar5;
  
  if (*(int *)(this + 8) == 0) {
    return 1;
  }
  bVar1 = false;
  for (puVar4 = *(ushort **)(this + 4); uVar3 = *puVar4, uVar3 != 0; puVar4 = puVar4 + 1) {
    uVar5 = (ushort)(byte)*param_1;
    if ((uVar5 == 0) || (uVar5 != uVar3)) goto LAB_00082fe6;
    param_1 = param_1 + 1;
  }
  uVar5 = (ushort)(byte)*param_1;
  bVar1 = true;
  uVar3 = 0;
LAB_00082fe6:
  if (uVar5 != 0) {
    cVar2 = (char)uVar3 - (char)uVar5;
    if (bVar1) {
      cVar2 = -1;
    }
    return (int)cVar2;
  }
  return (uint)(uVar3 != 0);
}

```

## target disasm
```
  00082fc0: ldr r2,[r0,#0x8]
  00082fc2: cbz r2,0x00082ff8
  00082fc4: ldr r2,[r0,#0x4]
  00082fc6: mov.w r12,#0x0
  00082fca: b 0x00082fd8
  00082fcc: ldrb r3,[r1,#0x0]
  00082fce: cbz r3,0x00082fe6
  00082fd0: cmp r3,r0
  00082fd2: bne 0x00082fe6
  00082fd4: adds r2,#0x2
  00082fd6: adds r1,#0x1
  00082fd8: ldrh r0,[r2,#0x0]
  00082fda: cmp r0,#0x0
  00082fdc: bne 0x00082fcc
  00082fde: ldrb r3,[r1,#0x0]
  00082fe0: mov.w r12,#0x1
  00082fe4: movs r0,#0x0
  00082fe6: cbz r3,0x00082ffc
  00082fe8: subs r0,r0,r3
  00082fea: cmp.w r12,#0x0
  00082fee: it ne
  00082ff0: mov.ne.w r0,#0xffffffff
  00082ff4: sxtb r0,r0
  00082ff6: bx lr
  00082ff8: movs r0,#0x1
  00082ffa: bx lr
  00082ffc: cmp r0,#0x0
  00082ffe: it ne
  00083000: mov.ne r0,#0x1
  00083002: bx lr
```
