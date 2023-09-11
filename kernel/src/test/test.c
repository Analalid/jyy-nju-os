// #include <stdio.h>
// #ifndef TEST
// // 框架代码中的 pmm_init (在 AbstractMachine 中运行)
// static void pmm_init() {
//     printf("================def test==================\n");
//   uintptr_t pmsize = ((uintptr_t)heap.end - (uintptr_t)heap.start);
//   printf("Got %d MiB heap: [%p, %p)\n", pmsize >> 20, heap.start, heap.end);
// }
// #else
// // 测试代码的 pmm_init ()
// static void pmm_init() {
//   char *ptr  = malloc(HEAP_SIZE);
//   heap.start = ptr;
//   heap.end   = ptr + HEAP_SIZE;
//   printf("Got %d MiB heap: [%p, %p)\n", HEAP_SIZE >> 20, heap.start, heap.end);
// }
// #endif