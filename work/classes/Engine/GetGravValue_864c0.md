# Engine::GetGravValue
elf 0x864c0 body 80
Sig: undefined __thiscall GetGravValue(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::GetGravValue() */

Engine * __thiscall AbyssEngine::Engine::GetGravValue(Engine *this)

{
  double dVar1;
  double dVar2;
  
  dVar1 = *(double *)(this + 0x4e0);
  if (*(int *)(**(int **)(this + 0x30) + 0x30) == 1) {
    dVar1 = -dVar1;
    dVar2 = -*(double *)(this + 0x4e8);
  }
  else {
    dVar2 = *(double *)(this + 0x4e8);
  }
  *(double *)(this + 0x4f8) = dVar1;
  *(double *)(this + 0x500) = dVar2;
  *(undefined8 *)(this + 0x508) = *(undefined8 *)(this + 0x4f0);
  return this + 0x4f8;
}

```

## target disasm
```
  000964c0: ldr r1,[r0,#0x30]
  000964c2: add.w r2,r0,#0x4e0
  000964c6: vldr.64 d16,[r2]
  000964ca: ldr r1,[r1,#0x0]
  000964cc: ldr r1,[r1,#0x30]
  000964ce: cmp r1,#0x1
  000964d0: bne 0x000964e4
  000964d2: add.w r1,r0,#0x4e8
  000964d6: vneg.f64 d16,d16
  000964da: vldr.64 d17,[r1]
  000964de: vneg.f64 d17,d17
  000964e2: b 0x000964ec
  000964e4: add.w r1,r0,#0x4e8
  000964e8: vldr.64 d17,[r1]
  000964ec: add.w r1,r0,#0x4f8
  000964f0: add.w r2,r0,#0x500
  000964f4: vstr.64 d16,[r1]
  000964f8: vstr.64 d17,[r2]
  000964fc: add.w r2,r0,#0x4f0
  00096500: add.w r0,r0,#0x508
  00096504: vldr.64 d16,[r2]
  00096508: vstr.64 d16,[r0]
  0009650c: mov r0,r1
  0009650e: bx lr
```
