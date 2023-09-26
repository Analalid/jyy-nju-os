#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <regex.h>
#include <stdint.h>
#define TABLE_SIZE 2048 
 #define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
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

void put_HashMap(HashTable* table, const char* key, int value) {
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
    return -1; // 未找到对应的值
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
  exit(EXIT_SUCCESS);
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
  exit(EXIT_SUCCESS);
}
void printfMap(){
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        HashEntry* entry = map->entries[i];
        if (entry != NULL) {
            printf("key: %s   value: %lf\n", entry->key, entry->value);
            // free(entry->key);
            // free(entry);
        }
    }
};
void putMapByString(char* substring){
    char * syscallName = getSyscall(substring);
    double t = getTimeUsed(substring);
    // int hash = hashFunction(syscallName);
    double v = get_HashMap(map, syscallName);
    put_HashMap(map, syscallName, t + (v + 1 < 0.00000001? 0 : v));
    printf("====%s   %lf\n ",syscallName, v);
    totalTimeCost += t;
}
void readTmpOutFile(int fd){
    // printf("%d\n", fd);
    char line[4096];  // 用于存储读取的行数据
    char ch;
    ssize_t bytesRead;
    int index = 0;
    while ((bytesRead = read(fd, &ch, 1)) > 0) {
      if (ch == '\n') {
          line[index] = '\0';  // 添加字符串结尾标志
          // printf("读取的行数据: %s\n", line);
          char* substring = strndup(&line[0], index);
          putMapByString(substring);
          free(substring);
          index = 0;  // 重置索引
      } else {
          line[index] = ch;
  //         printf("================================================\n");
  // printfMap();
          index++;
      }
  }
}
int main(int argc, char *argv[]) {
  map = createHashTable();
  char *exec_argv[] = {  "strace","-T","wc","sperf.c",NULL, };
  char *exec_envp[] = { "PATH=/bin", NULL, };
  close(2);
  int pipefd[2];
  if(pipe(pipefd) == -1) perror("create file failed!\n");
  setbuf(stdout, NULL);
  int p = fork();
  if(p < 0){
    perror("create child process error!\n");
  }else if(p > 0){
    wait(NULL);
    printf("father process begin!\n");
    readTmpOutFile(pipefd[0]);
          
  }else{
    if(dup2(pipefd[1], STDERR_FILENO) < 0) perror("fail!\n");
    execve("/bin/strace",     exec_argv, exec_envp);
    perror(argv[0]);
  }
}
