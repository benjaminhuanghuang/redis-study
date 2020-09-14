#ifndef __SDS_H
#define __SDS_H

typedef char *sds;

struct sdshdr_3_2
{
  int len;  // 4 byts, spaces used in buf
  int free; // 4 bytes, free space in buf
  char buf[];
};

struct __attribute__((__packed__)) sdshdr5
{
  unsigned char flags; /* 3 lsb of type, and 5 msb of string length */
  char buf[];
};

struct __attribute__((__packed__)) sdshdr8
{
  uint8_t len;         /* used */
  uint8_t alloc;       /* excluding the header and null terminator */
  unsigned char flags; /* 3 lsb of type, 5 unused bits */
  char buf[];
};

struct __attribute__((__packed__)) sdshdr16
{
  uint16_t len;        /* used */
  uint16_t alloc;      /* excluding the header and null terminator */
  unsigned char flags; /* 3 lsb of type, 5 unused bits */
  char buf[];
};

struct __attribute__((__packed__)) sdshdr32
{
  uint32_t len;        /* used */
  uint32_t alloc;      /* excluding the header and null terminator */
  unsigned char flags; /* 3 lsb of type, 5 unused bits */
  char buf[];
};

struct __attribute__((__packed__)) sdshdr64
{
  uint64_t len;        /* used */
  uint64_t alloc;      /* excluding the header and null terminator */
  unsigned char flags; /* 3 lsb of type, 5 unused bits */
  char buf[];
};

#define SDS_TYPE_5 0
#define SDS_TYPE_8 1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_64 4

#define SDS_TYPE_MASK 7 // 0b111
#define SDS_TYPE_BITS 3 // in flages, 3 lsb is the type

//
#define SDS_HDR_VAR(T, s) struct sdshdr##T *sh = (void *)((s) - (sizeof(struct sdshdr##T)));
//
#define SDS_HDR(T, s) ((struct sdshdr##T *)((s) - (sizeof(struct sdshdr##T))))
// in flags of sdshdr5, the 5 msb of string length
#define SDS_TYPE_5_LEN(f) ((f) >> SDS_TYPE_BITS)

static inline size_t sdslen(const sds s)
{
  unsigned char flags = s[-1];
  switch (flags & SDS_TYPE_MASK)
  {
  case SDS_TYPE_5:
    return SDS_TYPE_5_LEN(flags);
  case SDS_TYPE_8:
    return SDS_HDR(8, s)->len;
  case SDS_TYPE_16:
    return SDS_HDR(16, s)->len;
  case SDS_TYPE_32:
    return SDS_HDR(32, s)->len;
  case SDS_TYPE_64:
    return SDS_HDR(64, s)->len;
  }
  return 0;
}

static inline size_t sdsavail(const sds s)
{
  unsigned char flags = s[-1];
  switch (flags & SDS_TYPE_MASK)
  {
  case SDS_TYPE_5:
  {
    return 0;
  }
  case SDS_TYPE_8:
  {
    SDS_HDR_VAR(8, s);
    return sh->alloc - sh->len;
  }
  case SDS_TYPE_16:
  {
    SDS_HDR_VAR(16, s);
    return sh->alloc - sh->len;
  }
  case SDS_TYPE_32:
  {
    SDS_HDR_VAR(32, s);
    return sh->alloc - sh->len;
  }
  case SDS_TYPE_64:
  {
    SDS_HDR_VAR(64, s);
    return sh->alloc - sh->len;
  }
  }
  return 0;
}

static inline void sdssetlen(sds s, size_t newlen)
{
  unsigned char flags = s[-1];
  switch (flags & SDS_TYPE_MASK)
  {
  case SDS_TYPE_5:
  {
    unsigned char *fp = ((unsigned char *)s) - 1;
    *fp = SDS_TYPE_5 | (newlen << SDS_TYPE_BITS);
  }
  break;
  case SDS_TYPE_8:
    SDS_HDR(8, s)->len = newlen;
    break;
  case SDS_TYPE_16:
    SDS_HDR(16, s)->len = newlen;
    break;
  case SDS_TYPE_32:
    SDS_HDR(32, s)->len = newlen;
    break;
  case SDS_TYPE_64:
    SDS_HDR(64, s)->len = newlen;
    break;
  }
}

static inline void sdsinclen(sds s, size_t inc)
{
  unsigned char flags = s[-1];
  switch (flags & SDS_TYPE_MASK)
  {
  case SDS_TYPE_5:
  {
    unsigned char *fp = ((unsigned char *)s) - 1;
    unsigned char newlen = SDS_TYPE_5_LEN(flags) + inc;
    *fp = SDS_TYPE_5 | (newlen << SDS_TYPE_BITS);
  }
  break;
  case SDS_TYPE_8:
    SDS_HDR(8, s)->len += inc;
    break;
  case SDS_TYPE_16:
    SDS_HDR(16, s)->len += inc;
    break;
  case SDS_TYPE_32:
    SDS_HDR(32, s)->len += inc;
    break;
  case SDS_TYPE_64:
    SDS_HDR(64, s)->len += inc;
    break;
  }
}

sds sdsnewlen(const void *init, size_t initlen);
sds sdsnew(const char *init);
sds sdsempty(void);
sds sdsdup(const sds s);
void sdsfree(sds s);
sds sdsgrowzero(sds s, size_t len);
sds sdscatlen(sds s, const void *t, size_t len);
sds sdscat(sds s, const char *t);
sds sdscatsds(sds s, const sds t);
sds sdscpylen(sds s, const char *t, size_t len);
sds sdscpy(sds s, const char *t);

#endif