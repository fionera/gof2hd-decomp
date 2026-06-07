# Radio::setMessages
elf 0x15501c body 48
Sig: undefined __thiscall setMessages(Radio * this, Array * param_1)

## decompile
```c

/* Radio::setMessages(Array<RadioMessage*>*) */

void __thiscall Radio::setMessages(Radio *this,Array *param_1)

{
  uint uVar1;
  
  *(Array **)this = param_1;
  if (param_1 != (Array *)0x0) {
    for (uVar1 = 0; uVar1 < *(uint *)param_1; uVar1 = uVar1 + 1) {
      RadioMessage::setRadio(*(RadioMessage **)(*(int *)(param_1 + 4) + uVar1 * 4),this);
    }
  }
  return;
}

```

## target disasm
```
  0016501c: push {r4,r5,r6,r7,lr}
  0016501e: add r7,sp,#0xc
  00165020: push.w r11
  00165024: mov r4,r1
  00165026: mov r5,r0
  00165028: cmp r1,#0x0
  0016502a: str r1,[r0,#0x0]
  0016502c: beq 0x00165046
  0016502e: movs r6,#0x0
  00165030: b 0x00165040
  00165032: ldr r0,[r4,#0x4]
  00165034: mov r1,r5
  00165036: ldr.w r0,[r0,r6,lsl #0x2]
  0016503a: blx 0x000774a0
  0016503e: adds r6,#0x1
  00165040: ldr r0,[r4,#0x0]
  00165042: cmp r6,r0
  00165044: bcc 0x00165032
  00165046: pop.w r11
  0016504a: pop {r4,r5,r6,r7,pc}
```
