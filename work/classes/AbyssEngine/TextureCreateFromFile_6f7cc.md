# AbyssEngine::TextureCreateFromFile
elf 0x6f7cc body 40
Sig: undefined __stdcall TextureCreateFromFile(Engine * param_1, char * param_2, _func_void_Image_ptr_void_ptr * param_3, void * param_4, uint * param_5, bool param_6, float param_7)

## decompile
```c

/* AbyssEngine::TextureCreateFromFile(AbyssEngine::Engine*, char const*, void
   (*)(AbyssEngine::Image*, void*), void*, unsigned int*, bool, float) */

undefined4
AbyssEngine::TextureCreateFromFile
          (Engine *param_1,char *param_2,_func_void_Image_ptr_void_ptr *param_3,void *param_4,
          uint *param_5,bool param_6,float param_7)

{
  float in_stack_00000008;
  
  TextureCreateFromFileIntern
            (param_1,param_2,param_3,param_4,param_5,in_stack_00000008,
             (AELoadedTexture *)in_stack_00000008,false);
  return 1;
}

```

## target disasm
```
  0007f7cc: push {r0,r1,r2,r3,r4,r6,r7,lr}
  0007f7ce: add r7,sp,#0x18
  0007f7d0: ldr.w lr,[r7,#0xc]
  0007f7d4: movs r4,#0x0
  0007f7d6: vldr.32 s0,[r7,#0x10]
  0007f7da: ldr.w r12,[r7,#0x8]
  0007f7de: strd r4,lr,[sp,#0x8]
  0007f7e2: vstr.32 s0,[sp,#0x4]
  0007f7e6: str.w r12,[sp,#0x0]
  0007f7ea: blx 0x0006f220
  0007f7ee: movs r0,#0x1
  0007f7f0: add sp,#0x10
  0007f7f2: pop {r4,r6,r7,pc}
```
