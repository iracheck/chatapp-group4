#include <stdio.h>
#include <logging.h>

void force_shutdown()
{
  send_log("FORCE SHUTDOWN");
}