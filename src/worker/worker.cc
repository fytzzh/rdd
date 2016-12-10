//
// Created by Harunobu Daikoku on 2015/10/26.
//
#include <iostream>
#include "worker/executor.h"

using namespace std;
using namespace msgpack;

int main(int argc, const char **argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " [job_port] [num_threads]" << endl;
    return 1;
  }
  int job_port = stoi(argv[1]);
  int num_threads = stoi(argv[2]);

  rpc::server job_server;
  std::unique_ptr<rpc::dispatcher> dp(new Executor("localhost", job_port, num_threads));
  job_server.serve(dp.get());
  job_server.listen("0.0.0.0", job_port);
  job_server.run(1);

  return 0;
}

