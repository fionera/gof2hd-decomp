# String::ConvertFromUTF8
elf 0x7270c body 70
Sig: undefined __thiscall ConvertFromUTF8(String * this)

## decompile
```c

/* AbyssEngine::String::ConvertFromUTF8() */

void __thiscall AbyssEngine::String::ConvertFromUTF8(String *this)

{
  void *pvVar1;
  char *pcVar2;
  ushort *puVar3;
  
  if (*(int *)(this + 4) == 0) {
    return;
  }
  pvVar1 = (void *)GetAEChar();
  pcVar2 = (char *)GetAEChar();
  puVar3 = (ushort *)getWCharFromUtf8(pcVar2,*(int *)(this + 8));
  Set(this,puVar3);
  operator_delete__(pvVar1);
  (*(code *)(DAT_001ab114 + 0x1ab118))(puVar3);
  return;
}

```

## target disasm
```
  0008270c: push {r4,r5,r6,r7,lr}
  0008270e: add r7,sp,#0xc
  00082710: push.w r11
  00082714: mov r4,r0
  00082716: ldr r0,[r0,#0x4]
  00082718: cbz r0,0x0008274c
  0008271a: mov r0,r4
  0008271c: blx 0x0006ef68
  00082720: mov r5,r0
  00082722: mov r0,r4
  00082724: blx 0x0006ef68
  00082728: ldr r1,[r4,#0x8]
  0008272a: blx 0x0006f598
  0008272e: mov r6,r0
  00082730: mov r0,r4
  00082732: mov r1,r6
  00082734: blx 0x0006f3f4
  00082738: mov r0,r5
  0008273a: blx 0x0006ebfc
  0008273e: mov r0,r6
  00082740: pop.w r11
  00082744: pop.w {r4,r5,r6,r7,lr}
  00082748: b.w 0x001ab108
  0008274c: pop.w r11
  00082750: pop {r4,r5,r6,r7,pc}
```
