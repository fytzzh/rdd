//
// Created by Harunobu Daikoku on 2016/04/04.
//

#include "worker/shuffle/fully_connected_server.h"

bool FullyConnectedServer::OnRecv(struct pollfd &pfd) {
  auto fd = pfd.fd;

  int owner_id;
  auto recvd = ReadSome(fd, &owner_id, sizeof(owner_id));
  if (recvd < 0) {
    if (recvd == CONNECTION_ERROR) {
      // error
      std::cerr << "SERVER: ERROR" << std::endl;
      // TODO: might be better to close all connections
      // Break();
    } else if (recvd == CONNECTION_CLOSED) {
      // TODO: do something
    }

    return false;
  }

  if (recvd == sizeof(int)) {
    msgpack::sbuffer sbuf;
    std::vector<std::unique_ptr<char[]>> refs;
    auto len = block_mgr_.GroupPackBlocks(partitions_by_owner_[owner_id], sbuf, refs);

    SendBuffer header(&len, sizeof(len));
    ScheduleSend(pfd, std::move(header));
    if (len > 0) {
      SendBuffer body(std::unique_ptr<char[]>(sbuf.release()), len);
      ScheduleSend(pfd, std::move(body));
    } else if (len < 0) {
      partitions_by_owner_.erase(owner_id);
    }
  }

  return true;
}

bool FullyConnectedServer::OnSend(struct pollfd &pfd, SendBuffer &send_buffer) {
  auto size = send_buffer.GetSize();
  auto sent = WriteSome(pfd.fd, send_buffer.Get(), size);
  if (sent < 0) {
    // error
    std::cerr << "SERVER could not send a block" << std::endl;
    return false;
  }

  send_buffer.Consumed(sent);

  return (sent == size);
}

bool FullyConnectedServer::OnClose(struct pollfd &pfd) {
  return true;
}

bool FullyConnectedServer::IsRunning() {
  return partitions_by_owner_.size() > 0;
}