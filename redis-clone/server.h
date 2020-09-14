#ifndef __REDIS_H
#define __REDIS_H

#include <stdlib.h>
#include <stdio.h>

#include "version.h" /* Version macro */


typedef struct client
{
  uint64_t id;

} client;

struct redisServer
{
  pid_t pid;        /* Main process pid. */
  char *configfile; /* Absolute config file path, or NULL */
};

/*-----------------------------------------------------------------------------
 * Extern declarations
 *----------------------------------------------------------------------------*/
extern struct redisServer server;        

#endif