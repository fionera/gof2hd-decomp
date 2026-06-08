# ResourceTexture::ResourceTexture
elf 0x11b6d2 body 60
Sig: undefined __thiscall ResourceTexture(ResourceTexture * this, String * param_1, float param_2)

## decompile
```c

/* AbyssEngine::ResourceTexture::ResourceTexture(AbyssEngine::String const&, float) */

ResourceTexture * __thiscall
AbyssEngine::ResourceTexture::ResourceTexture(ResourceTexture *this,String *param_1,float param_2)

{
  void *pvVar1;
  int iVar2;
  void *pvVar3;
  undefined4 in_r2;
  
  pvVar1 = (void *)String::GetAEChar();
  iVar2 = *(int *)(param_1 + 8);
  pvVar3 = operator_new__(iVar2 + 1U);
  *(void **)this = pvVar3;
  __aeabi_memcpy(pvVar3,pvVar1,iVar2 + 1U);
  *(undefined4 *)(this + 4) = in_r2;
  operator_delete__(pvVar1);
  return this;
}

```

## target disasm
```
  0012b6d2: push {r4,r5,r6,r7,lr}
  0012b6d4: add r7,sp,#0xc
  0012b6d6: push.w r8
  0012b6da: mov r6,r0
  0012b6dc: mov r0,r1
  0012b6de: mov r8,r2
  0012b6e0: mov r5,r1
  0012b6e2: blx 0x0006ef68
  0012b6e6: mov r4,r0
  0012b6e8: ldr r0,[r5,#0x8]
  0012b6ea: adds r5,r0,#0x1
  0012b6ec: mov r0,r5
  0012b6ee: blx 0x0006ec08
  0012b6f2: mov r1,r4
  0012b6f4: mov r2,r5
  0012b6f6: str r0,[r6,#0x0]
  0012b6f8: blx 0x0006f1e4
  0012b6fc: mov r0,r4
  0012b6fe: str.w r8,[r6,#0x4]
  0012b702: blx 0x0006ebfc
  0012b706: mov r0,r6
  0012b708: pop.w r8
  0012b70c: pop {r4,r5,r6,r7,pc}
```
