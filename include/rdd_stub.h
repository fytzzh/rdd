//
// Created by Harunobu Daikoku on 2015/11/05.
//

#ifndef SLAVERDD_RDD_STUB_H
#define SLAVERDD_RDD_STUB_H

class RDDContext;

#include <unordered_map>
#include <vector>

class RDDStub {
 public:

  RDDStub(RDDContext &rc_, int rdd_id_)
      : rc_(rc_), rdd_id_(rdd_id_) { }

  RDDStub(RDDContext &rc, int rdd_id, const std::unordered_map<int, std::vector<int>> &partition_ids)
      : rc_(rc), rdd_id_(rdd_id), partitions_by_owner_(partition_ids) { }

  virtual ~RDDStub();

  void AddPartition(int owner, int partition_id);

  void GetPartitionIDsByOwner(int owner, std::vector<int> &partition_ids);

  void GetOwners(std::vector<int> &owners) const;

  void Print() const;

 protected:
  int rdd_id_;
  std::unordered_map<int, std::vector<int>> partitions_by_owner_;
  RDDContext &rc_;

};


#endif //SLAVERDD_RDD_STUB_H
