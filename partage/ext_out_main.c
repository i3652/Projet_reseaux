#include "extremite.h"

int main (int argc, char** argv){

  int tunfd = tun_alloc(argv[1]);
  char config[2000];
  sprintf(config, "./configure-tun.sh %s %s", argv[1], argv[2]);

  ext_out(tunfd, "123");

  return 0;
}
