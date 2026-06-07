# PaintCanvas::MaterialCreate
elf 0x7c13c body 60
Sig: undefined __stdcall MaterialCreate(uint * param_1, BlendMode param_2, uint param_3, ushort param_4)

## decompile
```c

/* AbyssEngine::PaintCanvas::MaterialCreate(unsigned int&, AbyssEngine::BlendMode, unsigned int,
   unsigned short) */

void AbyssEngine::PaintCanvas::MaterialCreate
               (int param_1,int *param_2,undefined4 param_3,undefined4 param_4)

{
  Material *this;
  
  this = operator_new(0x74);
  Material::Material(this);
  *(undefined4 *)this = param_4;
  *(undefined4 *)(this + 0x20) = param_3;
  ArrayAdd<AbyssEngine::Material*>(this,(Array *)(param_1 + 0x174));
  *param_2 = *(int *)(param_1 + 0x174) + -1;
  return;
}

```

## target disasm
```
  0008c13c: push {r4,r5,r6,r7,lr}
  0008c13e: add r7,sp,#0xc
  0008c140: push {r8,r9,r11}
  0008c144: mov r5,r0
  0008c146: movs r0,#0x74
  0008c148: mov r9,r3
  0008c14a: mov r6,r2
  0008c14c: mov r8,r1
  0008c14e: blx 0x0006eb24
  0008c152: mov r4,r0
  0008c154: blx 0x0006fc34
  0008c158: add.w r1,r5,#0x174
  0008c15c: mov r0,r4
  0008c15e: str.w r9,[r4,#0x0]
  0008c162: str r6,[r4,#0x20]
  0008c164: blx 0x0006fc40
  0008c168: ldr.w r0,[r5,#0x174]
  0008c16c: subs r0,#0x1
  0008c16e: str.w r0,[r8,#0x0]
  0008c172: pop.w {r8,r9,r11}
  0008c176: pop {r4,r5,r6,r7,pc}
```
