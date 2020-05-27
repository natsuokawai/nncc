#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct Token Token;
struct Token {
  TokenKind kind; // トークンの種類
  Token *next;    // 次のトークン
  long val;       // 数値（TK_NUM のとき）
  char *str;      // 文字列（TK_RESERVED のとき）
  int len;        // トークンの長さ
};

static Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}

static Token *tokenize(char *p) {
  Token head = {};
  Token *cur = &head;

  while (*p) {
    if (*p == ' ') {
      p++;
      continue;
    }

    if (isdigit(*p)) {
      // 数値が何桁続くかわからないのでとトークンだけ先に作る
      // その後 strtol で数値を取得し、数字の桁数だけポインタをインクリメントする
      cur = new_token(TK_NUM, cur, p, 0);
      char *q = p;
      cur->val = strtol(p, &p, 10);
      cur->len = p -q;
      continue;
    }

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    error("invalid token");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}

static bool equal(Token *tok, char *s) {
  return strlen(s) == tok->len && !strncmp(tok->str, s, tok->len);
}

// tok->str が s の場合は次のトークンを返す
static Token *skip(Token *tok, char *s) {
  if (!equal(tok, s)) error("expected '%s'", s);
  return tok->next;
}

static long get_number(Token *tok) {
  if (tok->kind != TK_NUM) error("expected a number");
  return tok->val;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("%s: invalid number of arguments\n", argv[0]);
    return 1;
  }

  Token *tok = tokenize(argv[1]);

  printf(".global main\n");
  printf("main:\n");

  printf("  mov $%ld, %%rax\n", get_number(tok));
  tok = tok->next;

  while (tok->kind != TK_EOF) {
    if (equal(tok, "+")) {
      tok = tok->next;
      printf("  add $%ld, %%rax\n", get_number(tok));
      tok = tok->next;
      continue;
    }
    
    // + でない場合は - を予期する
    tok = skip(tok, "-");
    printf("  sub $%ld, %%rax\n", get_number(tok));
    tok = tok->next;
  }

  printf("  ret\n");
  return 0;
}
