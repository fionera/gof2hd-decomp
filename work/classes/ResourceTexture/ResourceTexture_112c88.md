# ResourceTexture::ResourceTexture
elf 0x112c88 body 50
Sig: undefined __thiscall ResourceTexture(ResourceTexture * this, char * param_1, float param_2)

## decompile
```c

/* AbyssEngine::ResourceTexture::ResourceTexture(char const*, float) */

ResourceTexture * __thiscall
AbyssEngine::ResourceTexture::ResourceTexture(ResourceTexture *this,char *param_1,float param_2)

{
  int iVar1;
  void *pvVar2;
  undefined4 in_r2;
  
  iVar1 = String::GetStringLength(param_1);
  pvVar2 = operator_new__(iVar1 + 1U);
  *(void **)this = pvVar2;
  __aeabi_memcpy(pvVar2,param_1,iVar1 + 1U);
  *(undefined4 *)(this + 4) = in_r2;
  return this;
}

```

## target disasm
```
  00122c88: push {r4,r5,r6,r7,lr}
  00122c8a: add r7,sp,#0xc
  00122c8c: push.w r8
  00122c90: mov r6,r0
  00122c92: mov r0,r1
  00122c94: mov r8,r2
  00122c96: mov r5,r1
  00122c98: blx 0x00075ec8
  00122c9c: adds r4,r0,#0x1
  00122c9e: mov r0,r4
  00122ca0: blx 0x0006ec08
  00122ca4: mov r1,r5
  00122ca6: mov r2,r4
  00122ca8: str r0,[r6,#0x0]
  00122caa: blx 0x0006f1e4
  00122cae: str.w r8,[r6,#0x4]
  00122cb2: mov r0,r6
  00122cb4: pop.w r8
  00122cb8: pop {r4,r5,r6,r7,pc}
```
