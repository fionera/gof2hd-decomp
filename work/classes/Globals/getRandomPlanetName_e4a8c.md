# Globals::getRandomPlanetName
elf 0xe4a8c body 82
Sig: undefined __stdcall getRandomPlanetName(void)

## decompile
```c

/* Globals::getRandomPlanetName() */

void Globals::getRandomPlanetName(void)

{
  FileRead *this;
  Station *this_00;
  void *pvVar1;
  
  this = operator_new(1);
  FileRead::FileRead(this);
  AbyssEngine::AERandom::nextInt(**(int **)(DAT_000f4aec + 0xf4aaa));
  this_00 = (Station *)FileRead::loadStation((int)this);
  Station::getName();
  if (this_00 != (Station *)0x0) {
    pvVar1 = (void *)Station::~Station(this_00);
    operator_delete(pvVar1);
  }
  pvVar1 = (void *)FileRead::~FileRead(this);
  operator_delete(pvVar1);
  return;
}

```

## target disasm
```
  000f4a8c: push {r4,r5,r6,r7,lr}
  000f4a8e: add r7,sp,#0xc
  000f4a90: push.w r11
  000f4a94: mov r5,r0
  000f4a96: movs r0,#0x1
  000f4a98: blx 0x0006eb24
  000f4a9c: mov r4,r0
  000f4a9e: blx 0x00072124
  000f4aa2: ldr r0,[0x000f4aec]
  000f4aa4: movs r1,#0x64
  000f4aa6: add r0,pc
  000f4aa8: ldr r0,[r0,#0x0]
  000f4aaa: ldr r0,[r0,#0x0]
  000f4aac: blx 0x00071848
  000f4ab0: mov r1,r0
  000f4ab2: mov r0,r4
  000f4ab4: blx 0x000737a4
  000f4ab8: mov r6,r0
  000f4aba: mov r0,r5
  000f4abc: mov r1,r6
  000f4abe: blx 0x000736a8
  000f4ac2: cbz r6,0x000f4ace
  000f4ac4: mov r0,r6
  000f4ac6: blx 0x0007360c
  000f4aca: blx 0x0006eb48
  000f4ace: mov r0,r4
  000f4ad0: blx 0x0007213c
  000f4ad4: blx 0x0006eb48
  000f4ad8: pop.w r11
  000f4adc: pop {r4,r5,r6,r7,pc}
```
