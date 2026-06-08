# String::operator+=
elf 0x72850 body 74
Sig: undefined __thiscall operator+=(String * this, char * param_1)

## decompile
```c

/* AbyssEngine::String::TEMPNAMEPLACEHOLDERVALUE(char const&) */

String * __thiscall AbyssEngine::String::operator+=(String *this,char *param_1)

{
  byte bVar1;
  ushort *puVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = *(int *)(this + 8);
  if (iVar3 == 0) {
    puVar2 = operator_new__(4);
    *(ushort **)(this + 4) = puVar2;
    bVar1 = *param_1;
    puVar2[1] = 0;
    *puVar2 = (ushort)bVar1;
    *(undefined4 *)(this + 8) = 1;
  }
  else {
    *(int *)(this + 8) = iVar3 + 1;
    iVar3 = realloc(*(undefined4 *)(this + 4),iVar3 * 2 + 4);
    *(int *)(this + 4) = iVar3;
    iVar4 = *(int *)(this + 8);
    bVar1 = *param_1;
    *(undefined2 *)(iVar3 + iVar4 * 2) = 0;
    *(ushort *)(iVar3 + iVar4 * 2 + -2) = (ushort)bVar1;
  }
  return this;
}

```

## target disasm
```
  00082850: push {r4,r5,r7,lr}
  00082852: add r7,sp,#0x8
  00082854: mov r5,r1
  00082856: ldr r1,[r0,#0x8]
  00082858: mov r4,r0
  0008285a: cbz r1,0x00082882
  0008285c: adds r0,r1,#0x1
  0008285e: str r0,[r4,#0x8]
  00082860: ldr r0,[r4,#0x4]
  00082862: movs r2,#0x4
  00082864: add.w r1,r2,r1, lsl #0x1
  00082868: blx 0x0006ecc8
  0008286c: str r0,[r4,#0x4]
  0008286e: movs r3,#0x0
  00082870: ldr r1,[r4,#0x8]
  00082872: ldrb r2,[r5,#0x0]
  00082874: strh.w r3,[r0,r1,lsl #0x1]
  00082878: add.w r0,r0,r1, lsl #0x1
  0008287c: strh r2,[r0,#-0x2]
  00082880: b 0x00082896
  00082882: movs r0,#0x4
  00082884: blx 0x0006ec08
  00082888: str r0,[r4,#0x4]
  0008288a: movs r2,#0x0
  0008288c: ldrb r1,[r5,#0x0]
  0008288e: strh r2,[r0,#0x2]
  00082890: strh r1,[r0,#0x0]
  00082892: movs r0,#0x1
  00082894: str r0,[r4,#0x8]
  00082896: mov r0,r4
  00082898: pop {r4,r5,r7,pc}
```
