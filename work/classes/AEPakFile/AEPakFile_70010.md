# AEPakFile::AEPakFile
elf 0x70010 body 30
Sig: undefined __thiscall AEPakFile(AEPakFile * this, FileInterface * param_1, int param_2, int param_3)

## decompile
```c

/* AEPakFile::AEPakFile(FileInterface*, int, int) */

void __thiscall AEPakFile::AEPakFile(AEPakFile *this,FileInterface *param_1,int param_2,int param_3)

{
  *(int *)this = *(int *)(DAT_00080030 + 0x8001e) + 8;
  *(FileInterface **)(this + 4) = param_1;
  *(int *)(this + 8) = param_2;
  *(int *)(this + 0xc) = param_3;
  *(undefined4 *)(this + 0x10) = 0;
  return;
}

```

## target disasm
```
  00080010: push {r4,r6,r7,lr}
  00080012: add r7,sp,#0x8
  00080014: ldr r4,[0x00080030]
  00080016: mov.w r12,#0x0
  0008001a: add r4,pc
  0008001c: ldr r4,[r4,#0x0]
  0008001e: adds r4,#0x8
  00080020: strd r4,r1,[r0,#0x0]
  00080024: add.w r1,r0,#0x8
  00080028: stm r1,{r2,r3,r12}
  0008002c: pop {r4,r6,r7,pc}
```
