#include "class.h"
// 000aa9bc: ldrb.w r0,[r0,#0x398] ; bx lr
unsigned char PlayerEgo_hasVolatileGoods(PlayerEgo* self) {
  return UC(self, 0x398);
}
