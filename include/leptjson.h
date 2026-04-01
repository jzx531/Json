#ifndef LEPTJSON_H
#define LEPTJSON_H

#include <stdlib.h>
/* JSON 值类型枚举 */
typedef enum {
    LEPT_NULL,      /* null */
    LEPT_FALSE,     /* false */
    LEPT_TRUE,      /* true */
    LEPT_NUMBER,    /* 数字 */
    LEPT_STRING,    /* 字符串 */
    LEPT_ARRAY,     /* 数组 */
    LEPT_OBJECT     /* 对象 */
} lept_type;

/* JSON 值结构体 */
typedef struct {
    // double n;       /* 数字值 */
    union {//使用union节省内存
        struct { char* s; size_t len; }s;  /* string: null-terminated string, string length */
        double n;                          /* number */
    }u;
    lept_type type; /* 值类型 */
} lept_value;

/* 解析状态码枚举 */
enum {
    LEPT_PARSE_OK = 0,              /* 解析成功 */
    LEPT_PARSE_EXPECT_VALUE,        /* 期望值 */
    LEPT_PARSE_INVALID_VALUE,       /* 无效值 */
    LEPT_PARSE_ROOT_NOT_SINGULAR,    /* 根节点不唯一 */
    LEPT_PARSE_NUMBER_TOO_BIG,       /* 数字过大 */
    LEPT_PARSE_MISS_QUOTATION_MARK,  /* 缺少引号 */
    LEPT_PARSE_INVALID_STRING_ESCAPE,/* 无效转义字符 */
    LEPT_PARSE_INVALID_STRING_CHAR  /* 无效字符串字符 */
};

#define lept_init(v) do { (v)->type = LEPT_NULL; } while(0)
/**
 * 解析 JSON 字符串
 * @param v    输出参数，解析后的 JSON 值
 * @param json JSON 字符串
 * @return 解析状态码
 */
int lept_parse(lept_value* v, const char* json);

void lept_free(lept_value* v);

/**
 * 获取 JSON 值类型
 * @param v JSON 值指针
 * @return 值类型
 */
lept_type lept_get_type(const lept_value* v);

#define lept_set_null(v) lept_free(v)

int lept_get_boolean(const lept_value* v);
void lept_set_boolean(lept_value* v, int b);

/**
 * 获取 JSON 字符串值
 * @param v JSON 值指针
 * @return 字符串值
 */
double lept_get_number(const lept_value* v);
void lept_set_number(lept_value* v, double n);

const char* lept_get_string(const lept_value* v);
size_t lept_get_string_length(const lept_value* v);
void lept_set_string(lept_value* v, const char* s, size_t len);

#endif
