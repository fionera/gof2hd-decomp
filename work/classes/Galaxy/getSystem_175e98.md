# Galaxy::getSystem
elf 0x175e98 body 16
Sig: undefined __thiscall getSystem(Galaxy * this, int param_1)

## decompile
```c

/* Galaxy::getSystem(int) */

undefined4 __thiscall Galaxy::getSystem(Galaxy *this,int param_1)

{
  undefined4 uVar1;
  
  if (param_1 < 0) {
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined4 *)((int)this->systems->data + param_1 * 4);
  }
  return uVar1;
}

```

## target disasm
```
  00185e98: cmp r1,#0x0
  00185e9a: iteee lt
  00185e9c: mov.lt r0,#0x0
  00185e9e: ldr.ge r0,[r0,#0x4]
  00185ea0: ldr.ge r0,[r0,#0x4]
  00185ea2: ldr.ge.w r0,[r0,r1,lsl #0x2]
  00185ea6: bx lr
```
