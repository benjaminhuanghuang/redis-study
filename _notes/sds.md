## Reference
- Redis源码阅读系列1-sds

- Simple Dynamic Strings
  - https://github.com/antirez/sds

- 一文带你快速搞懂动态字符串SDS，面试不再懵逼

- Redis(5.0.3)源码分析之sds对象 [⭐️]

- redis源码分析 - string SDS [⭐️](https://www.geek1992.com/2020/04/27/blog19/)


## 
sds在redis中其实就是一个char*类型的别名
```
typedef char *sds;
```
sds指向的字符串的存储格式具有一定的规则，即在字符串数据之前存储了相应的头部信息，这些头部信息包含了
1. alloc 分配的内存空间长度
2. len   有效字符串长度
3. flags 头部类型。

redis中有sdshdr5，sdshdr8，sdshdr16，sdshdr32，sdshdr64这5类头部类型
```
  struct __attribute__ ((__packed__)) sdshdr5 {
      unsigned char flags; /* 3 lsb of type, and 5 msb of string length */
      char buf[];
  };

  struct __attribute__ ((__packed__)) sdshdr8 {
    uint8_t len;    /* used */
    uint8_t alloc;  /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
  };
```
定义这5种类型是为了尽量减少sdshdr占用的空间
sdshdr8, sdshdr16, sdshdr32, sdshdr64类型类似，仅len与alloc字段根据头部类型使用不同的类型，
sdshdr8使用uint8_t, sdshdr16使用uint16_t，sdshdr32使用uint32_t，sdshdr64使用uint64_t。

sdshdr5与其它头部类型不同，没有len与alloc字段，因此不能为字符串分配空间，字符串实际长度保存在flags字段的高5bits，
因此长度小于32.


__attribute__ ((__packed__)) 的作用是告诉GCC编译器，不要对此结构体做内存对齐。在C/C++的一个struct
创建时，会进行字节对齐操作，使得结构体的大小比字段占用的字节要多一些。Redis为了能直接从bug找到flags，
告诉编译器使用紧凑排练方式，从而可以使用buf - 1找到flags

header定义种最后一个字段 buf[]是C语言的一种特殊写法，成为柔性数组，只能定义在一个struct的最后一个字段上。
buf仅仅表示flags后面是一个字符数组，buf字段本身不占用header的内存空间。sizeof(struct sdshdr_16) 为5bytes

上层代码使用却是sds，是个char*，是怎么提取出len、alloc等字段信息呢？
其实sds是直接指向结构体里的buf数组。当取len等字段信息，只需要减去结构体长度，回退一下指针就行。

通过类型T和字符串的开始字节，获取字符串头部的开始位置，并赋值给sh指针
```
#define SDS_HDR_VAR(T,s) struct sdshdr##T *sh = (void*)((s)-(sizeof(struct sdshdr##T)));
```
通过类型T和字符串的开始字节，f返回字符串头部指针
```
#define SDS_HDR(T,s) ((struct sdshdr##T *)((s)-(sizeof(struct sdshdr##T))))
```
获取sdshdr5类型字符串的长度
```
#define SDS_TYPE_5_LEN(f) ((f)>>SDS_TYPE_BITS)
```

## Create string
Sample
```
 mystring = sdsnewlen("abc",3);
```

Souce code in sds.c
```
 sds sdsnewlen(const void *init, size_t initlen) {
```
