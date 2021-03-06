//
// Created by Harunobu Daikoku on 2015/12/04.
//

#ifndef PROJECT_BI_GRAM_MAPPER_H
#define PROJECT_BI_GRAM_MAPPER_H

#include <mapper.h>

class BiGramMapper:
    public Mapper<std::string, int, long long int, std::string> {

 public:

  virtual void Map(
      google::dense_hash_map<std::string, std::vector<int>> &kvs,
      const long long int &key,
      const std::string &value) override;

};

extern "C" std::unique_ptr<Mapper<std::string, int, long long int, std::string>> Create() {
  return std::unique_ptr<Mapper<std::string, int, long long int, std::string>>(new BiGramMapper);
}


#endif //PROJECT_BI_GRAM_MAPPER_H
