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

#define LEPT_KEY_NOT_EXIST ((size_t)-1)

typedef struct lept_value lept_value;
typedef struct lept_member lept_member;
/* JSON 值结构体 */
struct lept_value{
    // double n;       /* 数字值 */
    union {//使用union节省内存
        struct { lept_member* m; size_t size,capacity; }o;   /* object: members, member count */
        struct { lept_value* e; size_t size,capacity; }a;    /* array:  elements, element count */
        struct { char* s; size_t len; }s;  /* string: null-terminated string, string length */
        double n;                          /* number */
    }u;
    lept_type type; /* 值类型 */
} ;

struct lept_member{
    char *k;size_t klen; /* member key string, key string length */
    lept_value v;  /* member value */
};

/* 解析状态码枚举 */
enum {
    LEPT_PARSE_OK = 0,              /* 解析成功 */
    LEPT_PARSE_EXPECT_VALUE,        /* 期望值 */
    LEPT_PARSE_INVALID_VALUE,       /* 无效值 */
    LEPT_PARSE_ROOT_NOT_SINGULAR,    /* 根节点不唯一 */
    LEPT_PARSE_NUMBER_TOO_BIG,       /* 数字过大 */
    LEPT_PARSE_MISS_QUOTATION_MARK,  /* 缺少引号 */
    LEPT_PARSE_INVALID_STRING_ESCAPE,/* 无效转义字符 */
    LEPT_PARSE_INVALID_STRING_CHAR,  /* 无效字符串字符 */
    LEPT_PARSE_INVALID_UNICODE_HEX,  /* 无效 Unicode 十六进制 */
    LEPT_PARSE_INVALID_UNICODE_SURROGATE, /* 无效 Unicode 代理对 */
    LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,  /* 缺少逗号或方括号 */
    LEPT_PARSE_MISS_KEY,             /* 缺少键 */
    LEPT_PARSE_MISS_COLON,          /* 缺少冒号 */
    LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET  /* 缺少逗号或花括号 */
};

#define lept_init(v) do { (v)->type = LEPT_NULL; } while(0)
/**
 * 解析 JSON 字符串
 * @param v    输出参数，解析后的 JSON 值
 * @param json JSON 字符串
 * @return 解析状态码
 */
int lept_parse(lept_value* v, const char* json);

char* lept_stringify(const lept_value* v, size_t* length);

void lept_copy(lept_value* dst, const lept_value* src);
void lept_move(lept_value* dst, lept_value* src);
void lept_swap(lept_value* lhs, lept_value* rhs);

void lept_free(lept_value* v);

/**
 * 获取 JSON 值类型
 * @param v JSON 值指针
 * @return 值类型
 */
lept_type lept_get_type(const lept_value* v);
int lept_is_equal(const lept_value* lhs, const lept_value* rhs);

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

void lept_set_array(lept_value* v, size_t capacity);
size_t lept_get_array_size(const lept_value* v);
size_t lept_get_array_capacity(const lept_value* v);
void lept_reserve_array(lept_value* v, size_t capacity);
void lept_shrink_array(lept_value* v);
void lept_clear_array(lept_value* v);
lept_value* lept_get_array_element(lept_value* v, size_t index);
lept_value* lept_pushback_array_element(lept_value* v);
void lept_popback_array_element(lept_value* v);
lept_value* lept_insert_array_element(lept_value* v, size_t index);
void lept_erase_array_element(lept_value* v, size_t index, size_t count);

void lept_set_object(lept_value* v, size_t capacity);
size_t lept_get_object_size(const lept_value* v);
size_t lept_get_object_capacity(const lept_value* v);
void lept_reserve_object(lept_value* v, size_t capacity);
void lept_shrink_object(lept_value* v);
void lept_clear_object(lept_value* v);
const char* lept_get_object_key(const lept_value* v, size_t index);
size_t lept_get_object_key_length(const lept_value* v, size_t index);
lept_value* lept_get_object_value(lept_value* v, size_t index);
size_t lept_find_object_index(const lept_value* v, const char* key, size_t klen);
lept_value* lept_find_object_value(lept_value* v, const char* key, size_t klen);
lept_value* lept_set_object_value(lept_value* v, const char* key, size_t klen);
void lept_remove_object_value(lept_value* v, size_t index);

void print_lept_value(const lept_value* v);

#endif


