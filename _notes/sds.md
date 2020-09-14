## Reference
- Redis源码阅读系列1-sds

## 
__attribute__ ((packed)) 
作用就是告诉编译器取消结构在编译过程中的优化对齐,按照实际占用字节数进行对齐


## Create string
Sample
```
 mystring = sdsnewlen("abc",3);
```

Souce code in sds.c
```
 sds sdsnewlen(const void *init, size_t initlen) {
```
