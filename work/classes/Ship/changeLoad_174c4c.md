# Ship::changeLoad
elf 0x174c4c body 44
Sig: undefined __thiscall changeLoad(Ship * this, int param_1)

## decompile
```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Ship::changeLoad(int) */

void __thiscall Ship::changeLoad(Ship *this,int param_1)

{
  undefined4 *puVar1;
  int iVar2;
  
  iVar2 = *(int *)(this + 0x10);
  puVar1 = (undefined4 *)(_FUN_00184c78 + 0x184c5c);
  *(int *)(this + 0x10) = param_1 + iVar2;
  iVar2 = (*(int *)(this + 0xc) - (param_1 + iVar2)) + *(int *)(this + 0x28);
  if (*(int *)(*(int *)*puVar1 + 0xdc) < iVar2) {
    *(int *)(*(int *)*puVar1 + 0xdc) = iVar2;
  }
  return;
}

```
## target disasm
```
  00184c4c: push {r7,lr}
  00184c4e: mov r7,sp
  00184c50: ldr r2,[0x00184c78]
  00184c52: ldrd r12,r3,[r0,#0xc]
  00184c56: add r1,r3
  00184c58: add r2,pc
  00184c5a: ldr.w lr,[r0,#0x28]
  00184c5e: str r1,[r0,#0x10]
  00184c60: sub.w r1,r12,r1
  00184c64: ldr r2,[r2,#0x0]
  00184c66: add r1,lr
  00184c68: ldr r0,[r2,#0x0]
  00184c6a: ldr.w r2,[r0,#0xdc]
  00184c6e: cmp r2,r1
  00184c70: it lt
  00184c72: str.w.lt r1,[r0,#0xdc]
  00184c76: pop {r7,pc}
```
