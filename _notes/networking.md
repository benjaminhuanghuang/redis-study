


## Reference


- createClient()
createClient 在事件中心为与客户端连接的套接字注册了 readQueryFrom-Client() 回调函数，而这也就是说当客户端有请求数据过来的时候，acceptTcpHandler() 会被调用。于是，我们找到了’set name Jhon’ 开始处理的地方。

```
typedef struct client {
  querybuf
  buffer
}
```



