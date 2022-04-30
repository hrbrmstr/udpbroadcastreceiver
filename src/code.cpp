#include <Rcpp.h>
using namespace Rcpp;

#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <unistd.h>
#include <functional>
#include <string.h>

#define BUF_SIZE 1024 // buffer size for incoming UDP broadcast payloads

/* Check for interrupt without long jumping */
void check_interrupt_fn(void *dummy) {
  R_CheckUserInterrupt();
}

int pending_interrupt() {
  return !(R_ToplevelExec(check_interrupt_fn, NULL));
}

// [[Rcpp::export(invisible = true)]]
void udp_path_logger(int port, std::string path) {

  std::ofstream file_stream;

  file_stream.open(path);

  struct sockaddr_in si_me, si_other;

  int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  int broadcast = 1;
  int reuse_port = 1;
  int reuse_address = 1;

  setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address));
#ifdef __APPLE__
  setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port));
#endif

  memset(&si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  si_me.sin_addr.s_addr = INADDR_ANY;

  ::bind(s, (sockaddr *)&si_me, sizeof(sockaddr));

  bool processing = true;

  while(processing) {

    char buf[BUF_SIZE];
    unsigned slen = sizeof(sockaddr);

    int n = recvfrom(s, buf, sizeof(buf)-1, 0, (sockaddr *)&si_other, &slen);

    file_stream << std::string(buf, 0, n) << std::endl;

    if (pending_interrupt()) {
      processing = false;
    }

  }

  file_stream.close();


  close(s);

}

// [[Rcpp::export(invisible = true)]]
SEXP udp_callback_logger(int port, Function callback) {

  struct sockaddr_in si_me, si_other;

  int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  int broadcast = 1;
  int reuse_port = 1;
  int reuse_address = 1;

  setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address));
#ifdef __APPLE__
  setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse_port, sizeof(reuse_port));
#endif

  memset(&si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  si_me.sin_addr.s_addr = INADDR_ANY;

  ::bind(s, (sockaddr *)&si_me, sizeof(sockaddr));

  bool processing = true;

  while(processing) {

    char buf[BUF_SIZE];
    unsigned slen = sizeof(sockaddr);

    int n = recvfrom(s, buf, sizeof(buf)-1, 0, (sockaddr *)&si_other, &slen);

    LogicalVector res = callback(CharacterVector(std::string(buf, 0, n)));

    if ((is_true(all(res))) || pending_interrupt()) {
      processing = false;
    }

  }

  close(s);

  return(R_NilValue);

}
