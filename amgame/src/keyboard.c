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
    int dir[] = {0, 0};
    switch(*key_names[event.keycode]){
      case 'W':
          dir[1] = -1;
      break;
      case 'S':
          dir[1] = 1;
      break;
      case 'A':
          dir[0] = -1;
      break;
      case 'D':
          dir[0] = 1;
          break;
      default:
        puts("anything you pressed\n");
    }
    int x = 100;
    printf("%d", x);
    movePos(dir[0], dir[1]);
    splash();
  }
}
