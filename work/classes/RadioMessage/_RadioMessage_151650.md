# RadioMessage::~RadioMessage
elf 0x151650 body 22
Sig: undefined __thiscall ~RadioMessage(RadioMessage * this)

## decompile
```c

/* RadioMessage::~RadioMessage() */

RadioMessage * __thiscall RadioMessage::~RadioMessage(RadioMessage *this)

{
  if (*(void **)(this + 0x1c) != (void *)0x0) {
    operator_delete__(*(void **)(this + 0x1c));
  }
  *(undefined4 *)(this + 0x1c) = 0;
  return this;
}

```

## target disasm
```
  00161650: push {r4,r6,r7,lr}
  00161652: add r7,sp,#0x8
  00161654: mov r4,r0
  00161656: ldr r0,[r0,#0x1c]
  00161658: cbz r0,0x0016165e
  0016165a: blx 0x0006ebfc
  0016165e: movs r0,#0x0
  00161660: str r0,[r4,#0x1c]
  00161662: mov r0,r4
  00161664: pop {r4,r6,r7,pc}
```
