
#include "server.h"

/* Global vars */
struct redisServer server; /* Server global state */

void redisAsciiArt(void)
{
#include "modules/asciilogo.h"
  char *buf = (char *)malloc(1024 * 16);
  const char *mode = "clone!";
  int port = 3721;
  long pid = 1234;

  snprintf(buf, 1024 * 16,
           ascii_logo,
           REDIS_VERSION,
           "00000",
           100,
           (sizeof(long) == 8) ? "64" : "32",
           mode,
           port,
           pid);
  printf("%s\n", buf);
  free(buf);
}

int main(int argc, char **argv)
{
  redisAsciiArt();
  return 0;
}