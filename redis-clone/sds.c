#include <stdlib.h>

#include "sds.h"
#include "sdsalloc.h"

const char *SDS_NOINIT = "SDS_NOINIT";

static inline char sdsReqType(size_t string_size)
{
    if (string_size < 1 << 5)
        return SDS_TYPE_5;
    if (string_size < 1 << 8)
        return SDS_TYPE_8;
    if (string_size < 1 << 16)
        return SDS_TYPE_16;
#if (LONG_MAX == LLONG_MAX)
    if (string_size < 1ll << 32)
        return SDS_TYPE_32;
    return SDS_TYPE_64;
#else
    return SDS_TYPE_32;
#endif
}

static inline int sdsHdrSize(char type)
{
    switch (type & SDS_TYPE_MASK)
    {
    case SDS_TYPE_5:
        return sizeof(struct sdshdr5);
    case SDS_TYPE_8:
        return sizeof(struct sdshdr8);
    case SDS_TYPE_16:
        return sizeof(struct sdshdr16);
    case SDS_TYPE_32:
        return sizeof(struct sdshdr32);
    case SDS_TYPE_64:
        return sizeof(struct sdshdr64);
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

sds sdsnewlen(const void *init, size_t initlen)
{
    void *sh;
    sds s;
    char type = sdsReqType(initlen);
    /* Empty strings are usually created in order to append. Use type 8
     * since type 5 is not good at this. */
    if (type == SDS_TYPE_5 && initlen == 0)
        type = SDS_TYPE_8;
    int hdrlen = sdsHdrSize(type);
    unsigned char *fp; /* flags pointer. */

    sh = s_malloc(hdrlen + initlen + 1);
    if (sh == NULL)
        return NULL;
    if (init == SDS_NOINIT)
        init = NULL;
    else if (!init)
        memset(sh, 0, hdrlen + initlen + 1);
    s = (char *)sh + hdrlen; // return the address of buf field
    fp = ((unsigned char *)s) - 1;
    switch (type)
    {
    case SDS_TYPE_5:
    {
        *fp = type | (initlen << SDS_TYPE_BITS);
        break;
    }
    case SDS_TYPE_8:
    {
        // *sh = (void *)((s) - (sizeof(struct sdshdr##T)));
        SDS_HDR_VAR(8, s);
        sh->len = initlen;
        sh->alloc = initlen;
        *fp = type;
        break;
    }
    case SDS_TYPE_16:
    {
        SDS_HDR_VAR(16, s);
        sh->len = initlen;
        sh->alloc = initlen;
        *fp = type;
        break;
    }
    case SDS_TYPE_32:
    {
        SDS_HDR_VAR(32, s);
        sh->len = initlen;
        sh->alloc = initlen;
        *fp = type;
        break;
    }
    case SDS_TYPE_64:
    {
        SDS_HDR_VAR(64, s);
        sh->len = initlen;
        sh->alloc = initlen;
        *fp = type;
        break;
    }
    }
    if (initlen && init)
        memcpy(s, init, initlen);
    s[initlen] = '\0';
    return s;
}

/* Create an empty (zero length) sds string. Even in this case the string
 * always has an implicit null term. */
sds sdsempty(void)
{
    return sdsnewlen("", 0);
}

/* Create a new sds string starting from a null terminated C string. */
sds sdsnew(const char *init)
{
    size_t initlen = (init == NULL) ? 0 : strlen(init);
    return sdsnewlen(init, initlen);
}

/* Duplicate an sds string. */
sds sdsdup(const sds s)
{
    return sdsnewlen(s, sdslen(s));
}

/* Free an sds string. No operation is performed if 's' is NULL. */
void sdsfree(sds s)
{
    if (s == NULL)
        return;
    s_free((char *)s - sdsHdrSize(s[-1]));
}

void sdsclear(sds s)
{
    sdssetlen(s, 0);
    s[0] = '\0';
}

sds sdsMakeRoomFor(sds s, size_t addlen)
{
    void *sh, *newsh;
    size_t avail = sdsavail(s);
    size_t len, newlen;
    char type, oldtype = s[-1] & SDS_TYPE_MASK;
    int hdrlen;

    /* Return ASAP if there is enough space left. */
    if (avail >= addlen)
        return s;

    len = sdslen(s);
    sh = (char *)s - sdsHdrSize(oldtype);
    newlen = (len + addlen);
    if (newlen < SDS_MAX_PREALLOC)
        newlen *= 2;
    else
        newlen += SDS_MAX_PREALLOC;

    type = sdsReqType(newlen);

    /* Don't use type 5: the user is appending to the string and type 5 is
     * not able to remember empty space, so sdsMakeRoomFor() must be called
     * at every appending operation. */
    if (type == SDS_TYPE_5)
        type = SDS_TYPE_8;

    hdrlen = sdsHdrSize(type);
    if (oldtype == type)
    {
        newsh = s_realloc(sh, hdrlen + newlen + 1);
        if (newsh == NULL)
            return NULL;
        s = (char *)newsh + hdrlen;
    }
    else
    {
        /* Since the header size changes, need to move the string forward,
         * and can't use realloc */
        newsh = s_malloc(hdrlen + newlen + 1);
        if (newsh == NULL)
            return NULL;
        memcpy((char *)newsh + hdrlen, s, len + 1);
        s_free(sh);
        s = (char *)newsh + hdrlen;
        s[-1] = type;
        sdssetlen(s, len);
    }
    sdssetalloc(s, newlen);
    return s;
}
