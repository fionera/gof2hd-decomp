# Engine::GetAccelValue
elf 0x86510 body 80
Sig: undefined __thiscall GetAccelValue(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::GetAccelValue() */

Engine * __thiscall AbyssEngine::Engine::GetAccelValue(Engine *this)

{
  double dVar1;
  double dVar2;
  
  dVar1 = *(double *)(this + 0x4b0);
  if (*(int *)(**(int **)(this + 0x30) + 0x30) == 1) {
    dVar1 = -dVar1;
    dVar2 = -*(double *)(this + 0x4b8);
  }
  else {
    dVar2 = *(double *)(this + 0x4b8);
  }
  *(double *)(this + 0x4c8) = dVar1;
  *(double *)(this + 0x4d0) = dVar2;
  *(undefined8 *)(this + 0x4d8) = *(undefined8 *)(this + 0x4c0);
  return this + 0x4c8;
}

```

## target disasm
```
  00096510: ldr r1,[r0,#0x30]
  00096512: add.w r2,r0,#0x4b0
  00096516: vldr.64 d16,[r2]
  0009651a: ldr r1,[r1,#0x0]
  0009651c: ldr r1,[r1,#0x30]
  0009651e: cmp r1,#0x1
  00096520: bne 0x00096534
  00096522: add.w r1,r0,#0x4b8
  00096526: vneg.f64 d16,d16
  0009652a: vldr.64 d17,[r1]
  0009652e: vneg.f64 d17,d17
  00096532: b 0x0009653c
  00096534: add.w r1,r0,#0x4b8
  00096538: vldr.64 d17,[r1]
  0009653c: add.w r1,r0,#0x4c8
  00096540: add.w r2,r0,#0x4d0
  00096544: vstr.64 d16,[r1]
  00096548: vstr.64 d17,[r2]
  0009654c: add.w r2,r0,#0x4c0
  00096550: add.w r0,r0,#0x4d8
  00096554: vldr.64 d16,[r2]
  00096558: vstr.64 d16,[r0]
  0009655c: mov r0,r1
  0009655e: bx lr
```
