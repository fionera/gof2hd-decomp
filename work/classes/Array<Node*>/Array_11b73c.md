# Array<Node*>::Array
elf 0x11b73c body 28
Sig: undefined __thiscall Array(Array<Node*> * this)

## decompile
```c

/* Array<Node*>::Array() */

Array<Node*> * __thiscall Array<Node*>::Array(Array<Node*> *this)

{
  undefined4 *puVar1;
  
  puVar1 = operator_new__(4);
  *puVar1 = 0;
  *(undefined4 *)this = 0;
  *(undefined4 **)(this + 4) = puVar1;
  *(undefined4 *)(this + 8) = 1;
  return this;
}

```

## target disasm
```
  0012b73c: push {r4,r6,r7,lr}
  0012b73e: add r7,sp,#0x8
  0012b740: mov r4,r0
  0012b742: movs r0,#0x4
  0012b744: blx 0x0006ec08
  0012b748: movs r2,#0x0
  0012b74a: movs r1,#0x1
  0012b74c: str r2,[r0,#0x0]
  0012b74e: strd r2,r0,[r4,#0x0]
  0012b752: mov r0,r4
  0012b754: str r1,[r4,#0x8]
  0012b756: pop {r4,r6,r7,pc}
```
