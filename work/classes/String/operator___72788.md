# String::operator[]
elf 0x72788 body 28
Sig: undefined __thiscall operator[](String * this, int param_1)

## decompile
```c

/* AbyssEngine::String::operator[](int) const */

int __thiscall AbyssEngine::String::operator[](String *this,int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(DAT_000827a4 + 0x82792);
  if (-1 < param_1) {
    if (param_1 < *(int *)(this + 8)) {
      iVar1 = *(int *)(this + 4) + param_1 * 2;
    }
    return iVar1;
  }
  return iVar1;
}

```

## target disasm
```
  00082788: mov r2,r0
  0008278a: ldr r0,[0x000827a4]
  0008278c: cmp r1,#0x0
  0008278e: add r0,pc
  00082790: ldr r0,[r0,#0x0]
  00082792: it lt
  00082794: bx.lt lr
  00082796: ldr r3,[r2,#0x8]
  00082798: cmp r3,r1
  0008279a: itt gt
  0008279c: ldr.gt r0,[r2,#0x4]
  0008279e: add.gt.w r0,r0,r1, lsl #0x1
  000827a2: bx lr
```
