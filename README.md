# Json

a simple JSON library for C/C++ based on https://github.com/miloyip/json-tutorial.git

## 解析流程

### 解析json
```mermaid
graph TD
    A[开始 lept_parse] --> B[初始化 Context 和 Value]
    B --> C[跳过空白字符 lept_parse_whitespace]
    C --> D{检查首字符}
    
    D -->|t / f / n| E[解析字面量<br/>true false null]
    D -->|双引号| F[解析字符串 lept_parse_string]
    D -->|左方括号| G[解析数组 lept_parse_array]
    D -->|左大括号| H[解析对象 lept_parse_object]
    D -->|数字或负号| I[解析数字 lept_parse_number]
    D -->|EOF| J[错误: 无值]

    F --> K[循环读取字符]
    K --> L{字符类型}
    L -->|双引号| M[字符串结束]
    L -->|反斜杠| N[处理转义字符]
    L -->|控制字符| O[错误: 非法字符]
    L -->|普通字符| P[入栈 context_push]
    
    N -->|u| Q[解析 Unicode 转 UTF-8]
    Q --> P
    N -->|其他| R[错误: 非法转义]

    G --> S[初始化数组]
    S --> T[递归解析 value]
    T --> U[压栈]
    U --> V{下一个字符}
    V -->|逗号| S
    V -->|右方括号| W[完成数组]

    H --> X[初始化对象]
    X --> Y[解析 key 字符串]
    Y --> Z[解析冒号]
    Z --> AA[解析 value]
    AA --> AB[压栈]
    AB --> AC{下一个字符}
    AC -->|逗号| X
    AC -->|右大括号| AD[完成对象]

    M --> AE[跳过空白]
    W --> AE
    AD --> AE
    AE --> AF{是否结束}
    AF -->|是| AG[返回 OK]
    AF -->|否| AH[错误: 多余内容]
```

### 生成json
```mermaid
graph TD
    A[开始 stringify] --> B[初始化 Context]
    B --> C[调用 stringify_value]
    
    C --> D{类型判断}
    D -->|NULL| E[写 null]
    D -->|BOOL| F[写 true false]
    D -->|NUMBER| G[格式化输出]
    D -->|STRING| H[字符串处理]
    D -->|ARRAY| I[写 左括号]
    D -->|OBJECT| J[写 左大括号]

    H --> K[写引号]
    K --> L{字符类型}
    L -->|特殊字符| M[转义输出]
    L -->|控制字符| N[Unicode 转义]
    L -->|普通字符| O[直接写]
    M --> P[结束引号]
    N --> P
    O --> P

    I --> Q{遍历数组}
    Q --> R[递归 stringify]
    R --> S{是否最后}
    S -->|否| T[写逗号]
    T --> Q
    S -->|是| U[写右括号]

    J --> V{遍历对象}
    V --> W[写 key]
    W --> X[写冒号]
    X --> Y[递归 stringify]
    Y --> Z{是否最后}
    Z -->|否| AA[写逗号]
    AA --> V
    Z -->|是| AB[写右大括号]

    P --> AC[返回字符串]
    U --> AC
    AB --> AC
```

## 测试结果

```Shell
gcc -Wall -ggdb -W -O -Iinclude  -I/usr/include/tirpc -o output/main build/leptjson.o build/test08.o  -Llib -lpthread -ltirpc
Build complete
./output/main
552/552 (100.00%) passed
```