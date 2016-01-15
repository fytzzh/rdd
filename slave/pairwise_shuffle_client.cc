//
// Created by Harunobu Daikoku on 2016/01/11.
//

#include <iostream>
#include "socket/socket_client.h"
#include "pairwise_shuffle_client.h"


void PairwiseShuffleClient::Start(int server_id, const std::string &server_addr, int server_port) {
  int sock_fd;
  SocketClient client(server_addr, std::to_string(server_port));
  std::cout << "connecting to " << server_addr << ":" << server_port << std::endl;

  if ((sock_fd = client.Connect()) < 0) {
    std::cerr << "could not connect to: " << server_addr << ":" << server_port << std::endl;
    return;
  }

  msgpack::sbuffer sbuf;
  PackBlocks(server_id, sbuf);

  if (client.WriteWithHeader(sock_fd, sbuf.data(), sbuf.size()) < 0) {
    std::cerr << "write failed" << std::endl;
    return;
  }
  free(sbuf.release());

  size_t len = 0;
  auto rbuf = client.ReadWithHeader(sock_fd, len);
  if (!rbuf) {
    std::cerr << "read failed" << std::endl;
    return;
  }

  UnpackBlocks(rbuf.get(), len);
}

void PairwiseShuffleClient::PackBlocks(int server_rank, msgpack::sbuffer &sbuf) {
  long len = 0;
  while (true) {
    auto block = block_mgr_.GetBlock(server_rank, len);
    if (len == -1) {
      break;
    }
    msgpack::pack(&sbuf, std::string(block.get(), len));
  }
}

void PairwiseShuffleClient::UnpackBlocks(const char *buf, long len) {
  msgpack::unpacker upc;
  upc.reserve_buffer(len);
  memcpy(upc.buffer(), buf, len);
  upc.buffer_consumed(len);

  msgpack::unpacked result;
  while (upc.next(&result)) {
    std::string received;
    result.get().convert(&received);
    auto block_size = received.length();
    std::unique_ptr<char[]> block(new char[block_size]);
    received.copy(block.get(), block_size);
    block_mgr_.PutBlock(my_rank_, block_size, std::move(block));
  }
}
