# Engine::ReloadShaders
elf 0x865c8 body 52
Sig: undefined __thiscall ReloadShaders(Engine * this)

## decompile
```c

/* AbyssEngine::Engine::ReloadShaders() */

void __thiscall AbyssEngine::Engine::ReloadShaders(Engine *this)

{
  int *piVar1;
  uint uVar2;
  
  for (uVar2 = 0; uVar2 < *(uint *)(this + 0x510); uVar2 = uVar2 + 1) {
    (**(code **)(**(int **)(*(int *)(this + 0x514) + uVar2 * 4) + 0x24))();
    piVar1 = *(int **)(*(int *)(this + 0x514) + uVar2 * 4);
    (**(code **)(*piVar1 + 8))(piVar1,this);
  }
  return;
}

```

## target disasm
```
  000965c8: push {r4,r5,r7,lr}
  000965ca: add r7,sp,#0x8
  000965cc: mov r4,r0
  000965ce: movs r5,#0x0
  000965d0: b 0x000965f2
  000965d2: ldr.w r0,[r4,#0x514]
  000965d6: ldr.w r0,[r0,r5,lsl #0x2]
  000965da: ldr r1,[r0,#0x0]
  000965dc: ldr r1,[r1,#0x24]
  000965de: blx r1
  000965e0: ldr.w r0,[r4,#0x514]
  000965e4: ldr.w r0,[r0,r5,lsl #0x2]
  000965e8: ldr r1,[r0,#0x0]
  000965ea: ldr r2,[r1,#0x8]
  000965ec: mov r1,r4
  000965ee: blx r2
  000965f0: adds r5,#0x1
  000965f2: ldr.w r0,[r4,#0x510]
  000965f6: cmp r5,r0
  000965f8: bcc 0x000965d2
  000965fa: pop {r4,r5,r7,pc}
```
