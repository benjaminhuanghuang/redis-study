## Reference
- Redis源码阅读系列16-server
- 曹工说Redis源码（7）-- redis server 的周期执行任务，到底要做些啥
- Redis源码阅读笔记-服务器启动和初始化




## server 的主干部分
```
int main(int argc, char **argv) {
   ...
   // 初始化服务器配置，主要是填充 redisServer 结构体中的各种参数
   initServerConfig();
   ...
   // 初始化服务器
   initServer();
   ...
   // 进入事件循环
   aeMain(server.el);
}
```
## initServerConfig()

在这里，创建了事件中心，是 Redis 的网络模块，如果你有学过 linux 下的网络编程，那么知道这里一定和 select/epoll/kqueue 相关。

接着，是初始化数据中心，我们平时使用 Redis 设置的键值对，就是存储在里面。这里不急着深入它是怎么做到存储我们的键值对的，接着往下看好了，因为我们主要是想把大致的脉络弄清楚。

在最后一段的代码中，Redis 给 listen fd 注册了回调函数 acceptTcpHandler，也就是说当新的客户端连接的时候，这个函数会被调用



- server 
```
  struct redisServer {

  }
```
- fill server object
```
initServerConfig();
```

- initSever()
  - 

- Process args

- serverlog

- initServer()
  Create data structor
  - 


- loadDataFromDisk

- aeMain(server.el);
el is the event loop. 
process event loop in while loop


