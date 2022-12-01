#include "extremite.h"

int main (int argc, char** argv){

  int tunfd = tun_alloc(argv[1]);
  char config[1600];
  sprintf(config, "./configure-tun.sh %s %s", argv[1], argv[2]);
  system(config);
  ext_in(tunfd, argv[3], "123");

  return 0;
}
