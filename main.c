#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// エラー報告をして処理を終了する
static void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// トークンの種類
typedef enum {
  TK_RESERVED, // 予約後
  TK_NUM,      // 数値
  TK_EOF       // End-of-file
} TokenKind;

typedef struct Token {
  TokenKind kind; // トークンの種類
  Token *next;    // 次のトークン
  long val;       // 数値（TK_NUM のとき）
  char *str;      // 文字列（TK_RESERVED のとき）
  int len;        // トークンの長さ
} Token;

static Token *tokenize(char *p) {
  
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("%s: invalid number of arguments\n", argv[0]);
    return 1;
  }

  char *p = argv[1];

  printf(".global main\n");
  printf("main:\n");
  printf("  mov $%ld, %%rax\n", strtol(p, &p, 10));

  while (*p) {
    if (*p == '+') {
      p++;
      printf("  add $%ld, %%rax\n", strtol(p, &p, 10));
      continue;
    }
    
    if (*p == '-') {
      p++;
      printf("  sub $%ld, %%rax\n", strtol(p, &p, 10));
      continue;
    }

    error("unexpected character: '%c'\n", *p);
    return 1;
  }


  printf("  ret\n");
  return 0;
}
