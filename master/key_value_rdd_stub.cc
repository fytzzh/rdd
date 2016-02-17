//
// Created by Harunobu Daikoku on 2015/11/05.
//

#include <jubatus/msgpack/rpc/future.h>
#include "rdd_rpc.h"
#include "master/rdd_context.h"

std::unique_ptr<KeyValuesRDDStub> KeyValueRDDStub::Map(const std::string &dl_mapper) {
  std::vector<msgpack::rpc::future> fs;
  int new_rdd_id = rc_.GetNewRddId();

  for (auto p : partition_ids_) {
    rc_.SetTimeout(p.first, 600);
    fs.push_back(rc_.Call("map", p.first, rdd_id_, dl_mapper, new_rdd_id));
  }

  for (auto f : fs) {
    if (f.get<rdd_rpc::Response>() != rdd_rpc::Response::OK) {
      return nullptr;
    }
  }

  std::unique_ptr<KeyValuesRDDStub> mapped(new KeyValuesRDDStub(rc_, new_rdd_id, partition_ids_));
  mapped->Shuffle();

  return std::move(mapped);
}

std::unique_ptr<KeyValuesRDDStub> KeyValueRDDStub::Map(const std::string &dl_mapper,
                                                       const std::string &dl_combiner,
                                                       bool overlap) {
  std::vector<msgpack::rpc::future> fs;
  int new_rdd_id = rc_.GetNewRddId();


  for (auto p : partition_ids_) {
    rc_.SetTimeout(p.first, 600);
    if (overlap) {
      std::vector<int> owners;
      fs.push_back(rc_.Call("map_with_shuffle", p.first, rdd_id_, dl_mapper, dl_combiner, owners, new_rdd_id));
    } else {
      fs.push_back(rc_.Call("map_with_combine", p.first, rdd_id_, dl_mapper, dl_combiner, new_rdd_id));
    }

  }

  for (auto f : fs) {
    if (f.get<rdd_rpc::Response>() != rdd_rpc::Response::OK) {
      return nullptr;
    }
  }

  std::unique_ptr<KeyValuesRDDStub> mapped(new KeyValuesRDDStub(rc_, new_rdd_id, partition_ids_));

  if (!overlap) {
    mapped->Shuffle();
  }

  return std::move(mapped);
}
