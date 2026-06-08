# ResourceMesh::ResourceMesh
elf 0x112cf0 body 48
Sig: undefined __thiscall ResourceMesh(ResourceMesh * this, char * param_1, ushort param_2, bool param_3)

## decompile
```c

/* AbyssEngine::ResourceMesh::ResourceMesh(char const*, unsigned short, bool) */

ResourceMesh * __thiscall
AbyssEngine::ResourceMesh::ResourceMesh
          (ResourceMesh *this,char *param_1,ushort param_2,bool param_3)

{
  int iVar1;
  void *pvVar2;
  
  this[6] = (ResourceMesh)param_3;
  *(ushort *)(this + 4) = param_2;
  iVar1 = String::GetStringLength(param_1);
  pvVar2 = operator_new__(iVar1 + 1U);
  *(void **)this = pvVar2;
  __aeabi_memcpy(pvVar2,param_1,iVar1 + 1U);
  return this;
}

```

## target disasm
```
  00122cf0: push {r4,r5,r6,r7,lr}
  00122cf2: add r7,sp,#0xc
  00122cf4: push.w r11
  00122cf8: strb r3,[r0,#0x6]
  00122cfa: mov r5,r0
  00122cfc: strh r2,[r0,#0x4]
  00122cfe: mov r0,r1
  00122d00: mov r4,r1
  00122d02: blx 0x00075ec8
  00122d06: adds r6,r0,#0x1
  00122d08: mov r0,r6
  00122d0a: blx 0x0006ec08
  00122d0e: mov r1,r4
  00122d10: mov r2,r6
  00122d12: str r0,[r5,#0x0]
  00122d14: blx 0x0006f1e4
  00122d18: mov r0,r5
  00122d1a: pop.w r11
  00122d1e: pop {r4,r5,r6,r7,pc}
```
