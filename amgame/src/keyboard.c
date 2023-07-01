#include <game.h>

#define KEYNAME(key) \
  [AM_KEY_##key] = #key,
static const char *key_names[] = {
  AM_KEYS(KEYNAME)
};

void print_key() {
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  ioe_read(AM_INPUT_KEYBRD, &event);
  if (event.keycode != AM_KEY_NONE && event.keydown) {
    puts("Key pressed: ");
    puts(key_names[event.keycode]);
    puts("\n");
    // printf(event.keycode);
    int dir[2];
    switch(event.keycode){
      case 'W':
          dir[0] = -1;
      break;
      case 'S':
          dir[0] = 1;
      break;
      case 'A':
          dir[1] = -1;
      break;
      case 'D':
          dir[1] = 1;
      break;
      default :
        return;
    }
    moveSplash(dir);
  }
}
