#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include <sys/time.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int frame_w = 0, frame_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval tval;
  gettimeofday(&tval, NULL);
  return tval.tv_sec * 1000 + tval.tv_usec / 1000;
}

// 读出一条事件信息, 将其写入`buf`中。最长写入`len`字节,若读出了有效的事件, 函数返回1, 否则返回0
int NDL_PollEvent(char *buf, int len) {
  int eve_f = open("/dev/events", O_RDONLY);
  size_t ret = read(eve_f, buf, len);
  if(close(eve_f) != 0) assert(0);
  if(ret == 0) return 0;
  return 1;
}

// 打开一张(*w) X (*h)的画布。如果*w和*h均为0, 则将系统全屏幕作为画布, 并将*w和*h分别设为系统屏幕的大小
void NDL_OpenCanvas(int *w, int *h) {
  int dispinfo_f = open("/proc/dispinfo", O_RDONLY);
  char disp_buf[32];
  int ret = read(dispinfo_f, disp_buf, 30);
  ret = sscanf(disp_buf, "WIDTH : %d\nHEIGHT : %d\n", &frame_w, &frame_h); // 读取窗口大小
  if(*w == 0 && *h == 0){
    *w = frame_w;
    *h = frame_h;
  }
  screen_w = *w;
  screen_h = *h;
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}

// 向画布`(x, y)`坐标处绘制`w*h`的矩形图像, 并将该绘制区域同步到屏幕上。图像像素按行优先方式存储在`pixels`中, 每个像素用32位整数以`00RRGGBB`的方式描述颜色
void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  // 默认画布在屏幕中央
  int canvas_x = (frame_w - screen_w) / 2;
  int canvas_y = (frame_h - screen_h) / 2;
  int off_x = canvas_x + x;
  int off_y = canvas_y + y;
  int fb_f = open("/dev/fb", O_WRONLY);
  for (int i = 0; i < h; i++){
    lseek(fb_f, (off_x + (off_y+i) * frame_w) * sizeof(uint32_t), SEEK_SET);
    write(fb_f, pixels + w*i, w*sizeof(uint32_t));
  }
  // lseek(fb_f, (off_x + off_y * frame_w) * sizeof(uint32_t), SEEK_SET);
  // write(fb_f, pixels, w * h * sizeof(uint32_t));
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  printf("gggggggggggggggg\n");
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
