#ifndef __SDS_H
#define __SDS_H

struct sds {
  int len;        // 4 byts, spaces used in buf
  int free;       // 4 bytes, free space in buf
  char buf[];
};


#define SDS_TYPE_5  0
#define SDS_TYPE_8  1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_64 4

#define SDS_TYPE_MASK 7
#define SDS_TYPE_BITS 3

#endif