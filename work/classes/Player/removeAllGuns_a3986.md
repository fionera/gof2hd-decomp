# Player::removeAllGuns
elf 0xa3986 body 32
Sig: undefined __thiscall removeAllGuns(Player * this)

## decompile
```c

/* Player::removeAllGuns() */

void __thiscall Player::removeAllGuns(Player *this)

{
  void *pvVar1;
  
  if (*(Array **)this != (Array *)0x0) {
    ArrayReleaseClasses<Array<Gun*>*>(*(Array **)this);
    if (*(Array<Array<Gun*>*> **)this != (Array<Array<Gun*>*> *)0x0) {
      pvVar1 = (void *)Array<Array<Gun*>*>::~Array(*(Array<Array<Gun*>*> **)this);
      operator_delete(pvVar1);
    }
  }
  *(undefined4 *)this = 0;
  return;
}

```
## target disasm
```
  000b3986: push {r4,r6,r7,lr}
  000b3988: add r7,sp,#0x8
  000b398a: mov r4,r0
  000b398c: ldr r0,[r0,#0x0]
  000b398e: cbz r0,0x000b39a0
  000b3990: blx 0x00072e44
  000b3994: ldr r0,[r4,#0x0]
  000b3996: cbz r0,0x000b39a0
  000b3998: blx 0x00072e50
  000b399c: blx 0x0006eb48
  000b39a0: movs r0,#0x0
  000b39a2: str r0,[r4,#0x0]
  000b39a4: pop {r4,r6,r7,pc}
```
