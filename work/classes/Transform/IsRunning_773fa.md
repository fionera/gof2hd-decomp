# Transform::IsRunning
elf 0x773fa body 42
Sig: undefined __thiscall IsRunning(Transform * this)

## decompile
```c

/* AbyssEngine::Transform::IsRunning() */

undefined4 __thiscall AbyssEngine::Transform::IsRunning(Transform *this)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  iVar3 = *(int *)(this + 0x104);
  uVar2 = *(uint *)(this + 0x110);
  iVar1 = *(int *)(this + 0x114);
  if ((((int)((iVar3 - iVar1) - (uint)(*(uint *)(this + 0x100) < uVar2)) < 0 !=
        (SBORROW4(iVar3,iVar1) != SBORROW4(iVar3 - iVar1,(uint)(*(uint *)(this + 0x100) < uVar2))))
      && (iVar3 = *(int *)(this + 0x10c),
         (int)((iVar1 - iVar3) - (uint)(uVar2 < *(uint *)(this + 0x108))) < 0 !=
         (SBORROW4(iVar1,iVar3) != SBORROW4(iVar1 - iVar3,(uint)(uVar2 < *(uint *)(this + 0x108)))))
      ) && (this[0xed] != (Transform)0x0)) {
    return 1;
  }
  return 0;
}

```

## target disasm
```
  000873fa: ldrd r3,r12,[r0,#0x100]
  000873fe: ldrd r2,r1,[r0,#0x110]
  00087402: subs r3,r3,r2
  00087404: sbcs.w r3,r12,r1
  00087408: bge 0x00087420
  0008740a: ldrd r3,r12,[r0,#0x108]
  0008740e: subs r2,r2,r3
  00087410: sbcs.w r1,r1,r12
  00087414: bge 0x00087420
  00087416: ldrb.w r0,[r0,#0xed]
  0008741a: cbz r0,0x00087420
  0008741c: movs r0,#0x1
  0008741e: bx lr
  00087420: movs r0,#0x0
  00087422: bx lr
```
