## Reference
- Redis源码阅读系列16-server
- 曹工说Redis源码（7）-- redis server 的周期执行任务，到底要做些啥

## key points
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

- init server

- loadDataFromDisk

- aeMain(server.el);
el is the event loop. 
process event loop in while loop


