#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  assert(s!= NULL);
  size_t ans = 0;
  while((*s++) != '\0')
    ans++;
  return ans;
}

char *strcpy(char *dst, const char *src) {
  assert((dst != NULL) && (src != NULL));
  char *ret = dst;
  while((*dst++ = *src++) != '\0');
  return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
  assert((dst != NULL) && (src != NULL));
  char *ret = dst;
  while(n-- && ((*dst++ = *src++) != '\0'));
  while(n--) 
    *dst++ = '\0';
  return ret;
}

char *strcat(char *dst, const char *src) {
  assert((dst != NULL) && (src != NULL));
  char *ret = dst;
  while(*dst++);
  while((*dst++ = *src++) != '\0');
  return ret;
}

int strcmp(const char *s1, const char *s2) {
  assert((s1 != NULL) && (s2 != NULL));
  while(!(*s1 - *s2) && (*s2 != '\0')){
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  assert((s1 != NULL) && (s2 != NULL));
  int ret = 0;
  while(n-- && !(ret = *s1 - *s2) && (*s2 != '\0')){
    s1++;
    s2++;
  }
  return ret;
}

void *memset(void *s, int c, size_t n) {
  assert(s != NULL);
  char *tmp = (char*)s;
  char t = c;
  while(n--)
    *tmp ++ = t;
  return s;
}

void *memmove(void *dst, const void *src, size_t n) { // ?
  assert((dst != NULL) && (src != NULL));
  char *dst_t = (char*)dst;
  char *src_t = (char*)src;
  if(dst_t <= src_t || dst_t > src_t + n)
    while(n--)
      *dst_t++ = *src_t++;
  else{
    dst_t += n-1;
    src_t += n-1;
    while(n--)
      *dst_t-- = *src_t--;
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  // assert((out != NULL) && (in != NULL));
  // char *dst = (char*)out;
  // char *src = (char*)in;
  // while(n--)
  //   *dst++ = *src++;
  // return out;
  panic("Not implemented");
}


int memcmp(const void *s1, const void *s2, size_t n) {
  assert((s1 != NULL) && (s2 != NULL));
  const char *tmp1 = (char*)s1;
  const char *tmp2 = (char*)s2;
  while(n--){
    if(*tmp1 != *tmp2)
      return ((*tmp1 < *tmp2) ? -1 : 1);
    tmp1++;
    tmp2++;
  }
  return 0;
}

#endif
