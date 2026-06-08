# String::operator[]
elf 0x72378 body 28
Sig: undefined __thiscall operator[](String * this, int param_1)

## decompile
```c

/* AbyssEngine::String::operator[](int) */

int __thiscall AbyssEngine::String::operator[](String *this,int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(DAT_00082394 + 0x82382);
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
  00082378: mov r2,r0
  0008237a: ldr r0,[0x00082394]
  0008237c: cmp r1,#0x0
  0008237e: add r0,pc
  00082380: ldr r0,[r0,#0x0]
  00082382: it lt
  00082384: bx.lt lr
  00082386: ldr r3,[r2,#0x8]
  00082388: cmp r3,r1
  0008238a: itt gt
  0008238c: ldr.gt r0,[r2,#0x4]
  0008238e: add.gt.w r0,r0,r1, lsl #0x1
  00082392: bx lr
```
