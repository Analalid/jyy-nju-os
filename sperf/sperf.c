#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <regex.h>
#include <stdint.h>
#include <time.h>
#define TABLE_SIZE 2048 
#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
//画图的宏定义
#define SYSCALL_INFO_MAX (5)
//设置终端展示时候的窗口高
#define SYSCALL_INFO_WINDOW_HEIGHT (60)
//设置终端展示时候的窗口高
#define SYSCALL_INFO_WINDOW_WIDTH (20)

#define syscall_info_show_format(color) ("\e["#color";37m%s\e[0m")
const char *syscall_info_show_formats[SYSCALL_INFO_MAX] = {syscall_info_show_format(42), syscall_info_show_format(45), syscall_info_show_format(43), syscall_info_show_format(44), syscall_info_show_format(46)};
#define syscall_info_show(idx, str) (fprintf(stdout, syscall_info_show_formats[(idx)], (str)))

#define syscall_info_show_move(opcode) (fprintf(stdout, "\e[1"#opcode))
//将当前光标上移n行，列数不变
void syscall_info_show_move_up(int idx) {
	for(int i = 0; i < idx; ++i) { syscall_info_show_move(A); }
}
//将当前光标下移n行，列数不变
void syscall_info_show_move_down(int idx) {
	for(int i = 0; i < idx; ++i) { syscall_info_show_move(B); }
}
//将当前光标左移n列，行数不变
void syscall_info_show_move_left(int idx) {
	for(int i = 0; i < idx; ++i) { syscall_info_show_move(D); }
}
//将当前光标右移n列，行数不变
void syscall_info_show_move_right(int idx) {
	for(int i = 0; i < idx; ++i) { syscall_info_show_move(C); }
}
//将光标默认移动到第0行，第0列
#define syscall_info_show_position_init() (fprintf(stdout, "\e[0;0H"))

static char* re_syscall = ("^[^\\(]+");
static char* re_time = ("[0-9]+\\.[0-9]+[^>]");
//简易哈希表
typedef struct{
  char* key;
  double value;
}HashEntry;
typedef struct{
  HashEntry** entries;
} HashTable;
//预定义
HashTable* map = NULL;
double totalTimeCost = 0;
int totalSysNum = 0;
unsigned int hashFunction(const char* key) {
    unsigned int hash = 0;
    int i;
    for (i = 0; key[i] != '\0'; i++) {
        hash = hash * 31 + key[i];
    }
    return hash % TABLE_SIZE; // 使用除留余数法计算哈希值
}
HashTable* createHashTable() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->entries = (HashEntry**)calloc(TABLE_SIZE, sizeof(HashEntry*));
    return table;
}

void put_HashMap(HashTable* table, const char* key, double value) {
    unsigned int hash = hashFunction(key);
    HashEntry* entry = (HashEntry*)malloc(sizeof(HashEntry));
    entry->key = strdup(key);
    entry->value = value;
    // 在哈希值对应的位置插入新的条目
    table->entries[hash] = entry;
}

double get_HashMap(HashTable* table, const char* key) {
    unsigned int hash = hashFunction(key);
    HashEntry* entry = table->entries[hash];

    // 在哈希值对应的位置查找条目
    if (entry != NULL && strcmp(entry->key, key) == 0) {
        return entry->value;
    }
    return 0; // 未找到对应的值
}
char* getSyscall(char *str){
  char* key;
  char* value;

  char *s = str;
  regex_t     regex;
  regmatch_t  pmatch[1];
  regoff_t    len;
if (regcomp(&regex, re_syscall,   REG_NEWLINE | REG_EXTENDED))
    exit(EXIT_FAILURE);
for (unsigned int i = 0; ; i++) {
  if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0))  break;
    len = pmatch[0].rm_eo - pmatch[0].rm_so;
    char *output = (char*)malloc((len + 1) * sizeof(char));
    sprintf(output, "%.*s", len, s + pmatch[0].rm_so);
    return output;
  }
  // exit(EXIT_SUCCESS);
  return 0;
}
double getTimeUsed(char* str){
  char* key;
  char* value;

  char *s = str;
  regex_t     regex;
  regmatch_t  pmatch[1];
  regoff_t    len;
if (regcomp(&regex, re_time,   REG_NEWLINE | REG_EXTENDED))
    exit(EXIT_FAILURE);
for (unsigned int i = 0; ; i++) {
  if (regexec(&regex, s, ARRAY_SIZE(pmatch), pmatch, 0))  break;
    len = pmatch[0].rm_eo - pmatch[0].rm_so;
    char *output = (char*)malloc((len + 1) * sizeof(char));
    sprintf(output, "%.*s", len, s + pmatch[0].rm_so);
    return strtod(output, NULL);
  }
  return 0;
}

// 比较函数，用于排序
int compareHashEntry(const void* a, const void* b) {
    const HashEntry* entry1 = *(const HashEntry**)a;
    const HashEntry* entry2 = *(const HashEntry**)b;
    // 根据 key 进行升序排序
    return entry1->value - entry2->value > 0 ? 1 : -1;
}
void drawBlock(char* key, double percent, int idx, int left_top_row, int left_top_col, int right_end_row, int right_end_col){
    printf("\033[2J");    // 清除屏幕
    // 计算所需的缓冲区大小
    int size = snprintf(NULL, 0, "%s(%.1f%%)", key, percent);
    // 分配足够大小的缓冲区
    char *result = (char*)malloc(size + 1);
    // 拼接 double 和 char* 字符串
    snprintf(result, size + 1, "%s(%.1f%%)", key, percent);
    // printf("拼接结果：%s\n", result);
    // 释放动态分配的内存
    syscall_info_show(idx, result);
    for(int i = left_top_row; i < right_end_row; ++i){
      for(int j = left_top_col; j < right_end_col; ++j){
        if(i == left_top_row && j == right_end_col - strlen(result)) break;
        syscall_info_show(idx, " ");
      }
      syscall_info_show_move_down(1);
      syscall_info_show_move_left(right_end_col - left_top_col);
    }
    free(result);
}
void printfMap(){
    //清除屏幕
    int i;
    HashEntry *dataArr[totalSysNum];
    int idx = 0;
    for (i = 0; i < TABLE_SIZE; i++) {
        HashEntry* entry = map->entries[i];
        if (entry != NULL) {
            dataArr[idx++] = entry;
        }
    }
    qsort(dataArr, idx, sizeof(HashEntry*), compareHashEntry);
    //初始化光标
    fflush(stdout);
    syscall_info_show_position_init();
    int area = SYSCALL_INFO_WINDOW_WIDTH * SYSCALL_INFO_WINDOW_HEIGHT;
    int x_1 = 0, y_1 = 0, x_2 = SYSCALL_INFO_WINDOW_WIDTH, y_2 = SYSCALL_INFO_WINDOW_HEIGHT; 
    for(int i = idx - 1; i >= 0 && i > idx - 1 - SYSCALL_INFO_MAX; --i){
      // printf("%d\n ================== \n", i);
      double percent = dataArr[i]->value / totalTimeCost;
      int endX = -1;
      int endY = -1;
      if(i & 1){
        endX = x_2;
        endY = area * percent / (endX - x_1) + y_1;
      }else{
        endY = y_2;
        endX = area * percent / (endY - y_1) + x_1;
      }
      printf("x_1 = %d, y_1 = %d, x_2 = %d, y_2 = %d, endX = %d, endY = %d       percent = %lf\n", x_1,y_1,x_2, y_2,endX, endY,  percent * 100);
      // drawBlock(dataArr[i]->key, 100, idx - 1 - i, x_1, y_1, x_2, y_2);
      // if(i != idx - 1){

      drawBlock(dataArr[i]->key, percent * 100, idx - 1 - i, x_1, y_1, endX, endY);
      // }
      if(i & 1){
        y_1 = endY;
      }else{
        x_1 = endX;
      }
      syscall_info_show_position_init();
      syscall_info_show_move_right(y_1);
      syscall_info_show_move_down(x_1);
      // break;
      if(i == idx - 2) break;
    }
    while(1);
    // printf("\033[0m");  // 重置文本格式
};
void putMapByString(char* substring){
    char * syscallName = getSyscall(substring);
    double v = get_HashMap(map, syscallName);
    //第一次的时候返回0
    if(v == 0) ++totalSysNum;
    double t = getTimeUsed(substring);
    put_HashMap(map, syscallName, t + v);
    totalTimeCost += t;
}
void readTmpOutFile(int fd){
    // 将读取端设置为非阻塞模式
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
        perror("Failed to set read end to non-blocking");
    }
    // printf("%d\n", fd);
    char line[4096];  // 用于存储读取的行数据
    char ch;
    ssize_t bytesRead;
    int index = 0;
    // close(fd[1]);
    unsigned int end_time = time(NULL) + 1;
    while(1){
      if((bytesRead = read(fd, &ch, 1)) > 0) {
        if (ch == '\n') {
            line[index] = '\0';  // 添加字符串结尾标志
            // printf("读取的行数据: %s\n", line);
            char* substring = strndup(&line[0], index);
            putMapByString(substring);
            free(substring);
            index = 0;  // 重置索引
        } else {
            line[index] = ch;
            index++;
        }
    }
    //计时器轮询
    unsigned int t = time(NULL);
    if(t > end_time){
      printfMap();
      return;
      end_time += 1;
    }
  }
}
int main(int argc, char *argv[]) {
  if(argc < 2){
    perror("argc too small!\n");
    return -1;
  }
  map = createHashTable();
  char* exec_argv[argc + 2];
  exec_argv[0] = "strace";
  exec_argv[1] = "-T";
  for(int i = 2; i < argc + 1; ++i){
    exec_argv[i] = argv[i - 1];
  }
  exec_argv[argc + 1] = NULL;
  // char *exec_argv[] = {  "strace","-T","wc","sperf.c",NULL, };
  char *exec_envp[] = { "PATH=/bin", NULL, };
  close(2);
  int pipefd[2];
  if(pipe(pipefd) == -1) perror("create file failed!\n");
  setbuf(stdout, NULL);
  int p = fork();
  if(p < 0){
    perror("create child process error!\n");
  }else if(p > 0){
    printf("father process begin!\n");
    readTmpOutFile(pipefd[0]);
    wait(NULL);
  }else{
    if(dup2(pipefd[1], STDERR_FILENO) < 0) perror("fail!\n");
    execve("/bin/strace",     exec_argv, exec_envp);
    perror(argv[0]);
  }
}
