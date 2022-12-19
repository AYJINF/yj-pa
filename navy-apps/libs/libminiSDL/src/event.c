#include <NDL.h>
#include <SDL.h>
#include <string.h> 
#include <assert.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  assert(0);
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char ndl_event[70];
  if(NDL_PollEvent(ndl_event, 70) == 0) return 0;
  if(ndl_event[1] == 'u') ev->type = SDL_KEYUP;
  else ev->type = SDL_KEYDOWN;
  char k_name[16];
  int pos = -1;
  while(ndl_event[3 + pos++] !='\n')
    k_name[pos] = ndl_event[3 + pos]; // 阿巴阿巴
  k_name[pos] = '\0';
  for(int i = 0; i < sizeof(keyname) / sizeof(char *); i++){
    if (strcmp(k_name, keyname[i]) == 0){
      ev->key.keysym.sym = i;
      return 1;
    }
  }
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  char ndl_event[70];
  while(NDL_PollEvent(ndl_event, 70) == 0);
  if(ndl_event[1] == 'u') event->type = SDL_KEYUP;
  else event->type = SDL_KEYDOWN;
  char k_name[16];
  int pos = -1;
  while(ndl_event[3 + pos++] !='\n')
    k_name[pos] = ndl_event[3 + pos]; // 阿巴阿巴
  k_name[pos] = '\0';
  for(int i = 0; i < sizeof(keyname) / sizeof(char *); i++){
    printf("k_name = %s, keyname[i] = %s\n", k_name, keyname[i]);
    if (strcmp(k_name, keyname[i]) == 0){
      event->key.keysym.sym = i;
      return 1;
    }
  }
  return 0;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  assert(0);
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  assert(0);
  return NULL;
}
