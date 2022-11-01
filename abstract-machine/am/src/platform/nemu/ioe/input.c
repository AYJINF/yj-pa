#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t tmp = inl(KBD_ADDR);
  kbd->keydown = (bool)(tmp >> 15);
  kbd->keycode = (int)(tmp & 0x7fff);
}
