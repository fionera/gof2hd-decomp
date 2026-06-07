# Station::Station
elf 0xa692c body 60
Sig: undefined __thiscall Station(Station * this, String param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* Station::Station(AbyssEngine::String, int, int, int, int) */

Station * __thiscall
Station::Station(Station *this,String *param_2,undefined4 param_3,undefined4 param_4,
                undefined4 param_5,undefined4 param_6)

{
  String *this_00;
  
  this_00 = (String *)AbyssEngine::String::String((String *)this);
  AbyssEngine::String::operator=(this_00,param_2);
  *(undefined4 *)(this + 0xc) = param_3;
  *(undefined4 *)(this + 0x10) = param_4;
  *(undefined4 *)(this + 0x20) = param_5;
  *(undefined4 *)(this + 0x18) = param_6;
  this[0x1c] = (Station)0x0;
  this[0x24] = (Station)0x0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  return this;
}

```

## target disasm
```
  000b692c: push {r4,r5,r6,r7,lr}
  000b692e: add r7,sp,#0xc
  000b6930: push.w r8
  000b6934: mov r8,r3
  000b6936: mov r6,r2
  000b6938: mov r5,r1
  000b693a: mov r4,r0
  000b693c: blx 0x0006efbc
  000b6940: mov r1,r5
  000b6942: blx 0x0006f2b0
  000b6946: ldrd r1,r0,[r7,#0x8]
  000b694a: strd r6,r8,[r4,#0xc]
  000b694e: str r1,[r4,#0x20]
  000b6950: str r0,[r4,#0x18]
  000b6952: movs r0,#0x0
  000b6954: strb r0,[r4,#0x1c]
  000b6956: strb.w r0,[r4,#0x24]
  000b695a: strd r0,r0,[r4,#0x28]
  000b695e: str r0,[r4,#0x30]
  000b6960: mov r0,r4
  000b6962: pop.w r8
  000b6966: pop {r4,r5,r6,r7,pc}
```
