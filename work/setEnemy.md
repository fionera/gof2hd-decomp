# Gun::setEnemy

ELF vaddr 0x1523c2, body 8 bytes
Signature: undefined __stdcall setEnemy(Player * param_1)

## Ghidra decompile
```c

/* WARNING: Control flow encountered bad instruction data */
/* Gun::setEnemy(Player*) */

void Gun::setEnemy(Player *param_1)

{
  int unaff_r4;
  char in_NG;
  char in_OV;
  undefined4 in_cr15;
  
  if (in_NG == in_OV) {
    software_interrupt(0x3b5f0);
  }
  else {
    coprocessor_store(8,in_cr15,unaff_r4 + -0x134);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

```

## Target disassembly (must match)
```
  001623c2: swige 0x3b5f0
  001623c6: stclt p8,cr15,[r4,#-0x134]
```

Verify: tools/try.sh work/src/setEnemy.cpp setEnemy 1523c2 32
