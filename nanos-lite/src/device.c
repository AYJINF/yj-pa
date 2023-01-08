#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  yield();
  size_t ret = len;
  char *c = (char *)buf;
  while(*c != '\0' && len--){
    putch(*c++);
  }
  return ret;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  yield();
  AM_INPUT_KEYBRD_T in_key = io_read(AM_INPUT_KEYBRD);
  size_t ret = 0;
  if(in_key.keycode == AM_KEY_NONE) return 0;
  if(in_key.keydown)
    ret = sprintf((char *)buf, "kd %s\n", keyname[in_key.keycode]);
  else 
    ret = sprintf((char *)buf, "ku %s\n", keyname[in_key.keycode]);
  if(ret > len) assert(0);
  return ret;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  int W = io_read(AM_GPU_CONFIG).width;
  int H = io_read(AM_GPU_CONFIG).height;
  size_t ret = sprintf(buf, "WIDTH : %d\nHEIGHT : %d\n", W, H);
  if(ret > len) assert(0);
  // memset(buf + ret, 0, len - ret);
  return ret;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  yield();
  int line_len = io_read(AM_GPU_CONFIG).width * sizeof(uint32_t);
  int off_y = offset / line_len;
  int off_x = (offset % line_len) / sizeof(uint32_t);
  io_write(AM_GPU_FBDRAW, off_x, off_y, (uint32_t *)buf, len / 4, 1, false);
  io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true); // 先绘图再向屏幕同步
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
