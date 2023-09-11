static int get2PowSize(int size);
static void *kalloc(size_t size);
static void kfree(void *ptr);
static void pmm_init();
static void buddy_init(uintptr_t start, uintptr_t end);
static int dfs(size_t size, size_t curSize);