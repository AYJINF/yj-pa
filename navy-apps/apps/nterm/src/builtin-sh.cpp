#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
  char t_cmd[256];
  strcpy(t_cmd, cmd);
  t_cmd[strlen(t_cmd) - 1] = '\0';

  char *filename = strtok(t_cmd, " ");

  int argv_num = 0;
  char *argv[32];
  char *arg = strtok(NULL, " ");
  while (arg)
  {
    argv[argv_num] = arg;
    argv_num++;
    assert(argv_num < 32);
    arg = strtok(NULL, " ");
  }
  argv[argv_num] = NULL;
  // if(*argv) {
  //   printf("oooooooooooooooo\n");
  //   printf("argv[0]=%s\n", argv[0]);
  // }

  execvp(filename, argv);
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  setenv("PATH", "/bin", 0); // 增加环境变量

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
