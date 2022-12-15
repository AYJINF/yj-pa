#include <fs.h>

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENTS, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. 修改这里的时候记得修改STRACE*/
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  [FD_EVENTS] = {"/dev/events", 0, 0, events_read, invalid_write},
#include "files.h"
};

int fs_open(const char *pathname, int flags, int mode){
  int file_num = sizeof(file_table) / sizeof(Finfo);
  for(int i = 0; i < file_num; i++){
    if(strcmp(file_table[i].name, pathname) == 0){
      file_table[i].open_offset = 0;
      if(file_table[i].read == NULL) file_table[i].read = ramdisk_read;
      if(file_table[i].write == NULL) file_table[i].write = ramdisk_write;
      return i;
    }
  }
  printf("The file '%s' is not found!\n", pathname);
  assert(0);
}

size_t fs_read(int fd, void *buf, size_t len){
  Finfo *f = &file_table[fd];
  if(f->open_offset == f->size && (void *)f->read == (void *)ramdisk_read) return 0; // 或许可删去
  size_t ret = 0;
  if(f->read){
    if(len > f->size - f->open_offset && (void *)f->read == (void *)ramdisk_read) len = f->size - f->open_offset;
    if((void *)f->read == (void *)ramdisk_read) printf("ramdisk\n");
    if((void *)f->read == (void *)events_read) printf("event\n");
    ret = f->read(buf, f->disk_offset + f->open_offset, len);
    f->open_offset += ret;
    return ret;
  }
  return -1; // 阿巴阿巴
}

size_t fs_write(int fd, const void *buf, size_t len){
  Finfo *f = &file_table[fd];
  size_t ret = 0;
  if(f->write){
    if(f->open_offset == f->size && (void *)f->write != (void *)serial_write) return 0;
    if(len > f->size - f->open_offset && (void *)f->write == (void *)ramdisk_write) len = f->size - f->open_offset;
    ret = f->write(buf, f->disk_offset + f->open_offset, len);
    f->open_offset += ret;
    return ret;
  }
  return -1; // 阿巴阿巴
}

size_t fs_lseek(int fd, size_t offset, int whence){
  Finfo *f = &file_table[fd];
  switch (whence)
  {
  case SEEK_SET:
    f->open_offset = offset;
    break;
  case SEEK_CUR:
    f->open_offset += offset;
    break;
  case SEEK_END:
    f->open_offset = f->size + offset;
    break;
  default:
    f->open_offset = whence + offset;
    break;
  }
  if(f->open_offset > f->size || f->open_offset < 0){
    printf("It's out of the bound of the file!\n");
    assert(0);
  }
  return f->open_offset; // 阿巴阿巴，好像找不到什么情况会返回-1，先留个tag
}

int fs_close(int fd){
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}