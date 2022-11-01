#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static char sprintf_buf[1024];

int printf(const char *fmt, ...) {
  assert(fmt != NULL);
  va_list ap;
  va_start(ap, fmt);
  int str_len = vsprintf(sprintf_buf, fmt, ap);
  va_end(ap);
  for(int i = 0; i < str_len; i++)
    putch(sprintf_buf[i]);
  return str_len;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  assert((out != NULL) && (fmt != NULL));
  char *dst = out;
  while(*fmt != '\0'){
    // if(*fmt != '%'){
    //   *dst++ = *fmt++;
    //   continue;
    // }
    // fmt++;
    // int flag_0 = 0, field_width = -1;
    // if(*fmt == '0'){
    //   flag_0 = 1;
    //   fmt++;
    // }
    // if((*fmt >= '0') && (*fmt <= '9')){
    //   field_width = atoi(fmt);
    //   while((*fmt >= '0') && (*fmt <= '9'))
    //     fmt++;
    // }
    // char *s; size_t s_len;
    // char num_tmp[15];
    // int64_t num_t; // to be modified for more formats
    // int i = 0;
    // int count = 0;
    // switch (*fmt)
    // {
    //   case '%':
    //     *dst++ = '%';
    //     break;
    //   case 's':
    //     s = va_arg(ap, char*);
    //     s_len = strlen(s);
    //     for(i = 0; i < field_width - s_len; i++)
    //       *dst++ = flag_0 ? '0' : ' ';
    //     while(s_len--)
    //       *dst++ = *s++;
    //     break;
    //   case 'd':
    //     num_t = va_arg(ap, int);
    //     if(num_t == 0)
    //       num_tmp[count++] = '0';
    //     if(num_t < 0){
    //       num_t *= -1;
    //       *dst++ = '-';
    //     }
    //     while(num_t){
    //       num_tmp[count++] = '0' + (num_t % 10);
    //       num_t /= 10;
    //     }
    //     for(i = 0; i < field_width - count; i++)
    //       *dst++ = flag_0 ? '0' : ' ';
    //     while(count--){
    //       *dst++ = num_tmp[count];
    //     }
    //     break;
    //   default:
    //     break;
    // }
    fmt++;
  }
  *dst = '\0';
  return dst - out;
}

int sprintf(char *out, const char *fmt, ...) {
  assert((out != NULL) && (fmt != NULL));
  va_list ap;
  va_start(ap, fmt);
  int out_len = vsprintf(out, fmt, ap);
  va_end(ap);
  return out_len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
