# Standing::rehabilitate
elf 0x11d750 body 44
Sig: undefined __thiscall rehabilitate(Standing * this, int param_1)

## decompile
```c

/* Standing::rehabilitate(int) */

void __thiscall Standing::rehabilitate(Standing *this,int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  switch(param_1) {
  case 0:
    **(undefined4 **)this = 0xffffffdd;
    return;
  case 1:
    **(undefined4 **)this = 0x23;
    return;
  case 2:
    iVar1 = *(int *)this;
    uVar2 = 0xffffffdd;
    break;
  case 3:
    iVar1 = *(int *)this;
    uVar2 = 0x23;
    break;
  default:
    return;
  }
  *(undefined4 *)(iVar1 + 4) = uVar2;
  return;
}

```

## target disasm
```
  0012d750: cmp r1,#0x3
  0012d752: it hi
  0012d754: bx.hi lr
  0012d756: tbb [pc,r1]
  0012d75e: ldr r0,[r0,#0x0]
  0012d760: mvn r1,#0x22
  0012d764: str r1,[r0,#0x0]
  0012d766: bx lr
  0012d768: ldr r0,[r0,#0x0]
  0012d76a: mvn r1,#0x22
  0012d76e: b 0x0012d774
  0012d770: ldr r0,[r0,#0x0]
  0012d772: movs r1,#0x23
  0012d774: str r1,[r0,#0x4]
  0012d776: bx lr
  0012d778: ldr r0,[r0,#0x0]
  0012d77a: movs r1,#0x23
  0012d77c: str r1,[r0,#0x0]
  0012d77e: bx lr
```
