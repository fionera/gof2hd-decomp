# ConfigReader::RegisterTokenReadFunction
elf 0x8734c body 72
Sig: undefined __thiscall RegisterTokenReadFunction(ConfigReader * this, String param_1, _func_void_ConfigReader_ptr_void_ptr * param_2, void * param_3)

## decompile
```c

/* AbyssEngine::ConfigReader::RegisterTokenReadFunction(AbyssEngine::String, void
   (*)(AbyssEngine::ConfigReader*, void*), void*) */

void __thiscall
AbyssEngine::ConfigReader::RegisterTokenReadFunction
          (ConfigReader *this,String *param_2,undefined4 param_3,undefined4 param_4)

{
  String *this_00;
  
  this_00 = operator_new(0x14);
  *(undefined4 *)this_00 = 0;
  *(undefined4 *)(this_00 + 4) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  *(undefined4 *)(this_00 + 8) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  *(undefined4 *)(this_00 + 0xc) = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(this_00 + 0x10) = 0;
  String::String(this_00);
  String::operator=(this_00,param_2);
  *(undefined4 *)(this_00 + 0xc) = param_3;
  *(undefined4 *)(this_00 + 0x10) = param_4;
  (*(code *)(DAT_001ab754 + 0x1ab758))(this_00,this);
  return;
}

```

## target disasm
```
  0009734c: push {r4,r5,r6,r7,lr}
  0009734e: add r7,sp,#0xc
  00097350: push {r8,r9,r11}
  00097354: mov r9,r0
  00097356: movs r0,#0x14
  00097358: mov r8,r3
  0009735a: mov r6,r2
  0009735c: mov r5,r1
  0009735e: blx 0x0006eb24
  00097362: vmov.i32 q8,#0x0
  00097366: mov r4,r0
  00097368: movs r1,#0x0
  0009736a: vst1.64 {d16,d17},[r0]!
  0009736e: str r1,[r0,#0x0]
  00097370: mov r0,r4
  00097372: blx 0x0006efbc
  00097376: mov r0,r4
  00097378: mov r1,r5
  0009737a: blx 0x0006f2b0
  0009737e: strd r6,r8,[r4,#0xc]
  00097382: mov r0,r4
  00097384: mov r1,r9
  00097386: pop.w {r8,r9,r11}
  0009738a: pop.w {r4,r5,r6,r7,lr}
  0009738e: b.w 0x001ab748
  001ab748: bx pc
```
