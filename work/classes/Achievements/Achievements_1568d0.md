# Achievements::Achievements
elf 0x1568d0 body 34
Sig: undefined __thiscall Achievements(Achievements * this)

## decompile
```c

/* Achievements::Achievements() */

Achievements * __thiscall Achievements::Achievements(Achievements *this)

{
  void *pvVar1;
  
  pvVar1 = operator_new__(0xb4);
  *(void **)this = pvVar1;
  pvVar1 = operator_new__(0xb4);
  *(void **)(this + 4) = pvVar1;
  this[0x22] = (Achievements)0x0;
  *(undefined2 *)(this + 0x20) = 0;
  return this;
}

```

## target disasm
```
  001668d0: push {r4,r6,r7,lr}
  001668d2: add r7,sp,#0x8
  001668d4: mov r4,r0
  001668d6: movs r0,#0xb4
  001668d8: blx 0x0006ec08
  001668dc: str r0,[r4,#0x0]
  001668de: movs r0,#0xb4
  001668e0: blx 0x0006ec08
  001668e4: str r0,[r4,#0x4]
  001668e6: movs r0,#0x0
  001668e8: strb.w r0,[r4,#0x22]
  001668ec: strh r0,[r4,#0x20]
  001668ee: mov r0,r4
  001668f0: pop {r4,r6,r7,pc}
```
