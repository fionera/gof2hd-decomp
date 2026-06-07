# MTitle::MTitle
elf 0x97984 body 16
Sig: undefined __thiscall MTitle(MTitle * this)

## decompile
```c

/* MTitle::MTitle() */

void __thiscall MTitle::MTitle(MTitle *this)

{
  int iVar1;
  
  iVar1 = DAT_000a7994;
  *(undefined4 *)(this + 0xc) = 100;
  *(int *)this = *(int *)(iVar1 + 0xa798e) + 8;
  return;
}

```

## target disasm
```
  000a7984: ldr r1,[0x000a7994]
  000a7986: movs r2,#0x64
  000a7988: str r2,[r0,#0xc]
  000a798a: add r1,pc
  000a798c: ldr r1,[r1,#0x0]
  000a798e: adds r1,#0x8
  000a7990: str r1,[r0,#0x0]
  000a7992: bx lr
```
