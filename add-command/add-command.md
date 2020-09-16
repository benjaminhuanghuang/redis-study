## Reference
- Learning Modern C By Playing with Redis: Implementing a New (and Almost Useless) Redis Command

##
1. Go the redisCommandTable in server.c


2. Add command into command table
```
  {"mycmd", myCommand, -1
    "descripion of the command",
    0, NULL, 0,0,0,0,0,0}
```

2. Add the function for command in sever.c
```
  void myCommand(client *c)
  {
    addReplyBulkBuffer(d, "abc", 3);
  }
```