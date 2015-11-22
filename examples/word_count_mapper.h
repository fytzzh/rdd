//
// Created by Harunobu Daikoku on 2015/11/20.
//

#ifndef SLAVERDD_WORD_COUNT_MAPPER_H
#define SLAVERDD_WORD_COUNT_MAPPER_H

#include <mapper.h>
#include <string>

class WordCountMapper: public Mapper<std::string, int, int, std::string> {
 public:

  virtual void
      Map(tbb::concurrent_unordered_map<std::string, tbb::concurrent_vector<int>> &kvs,
          const int &key, const std::string &value) override;
};

extern "C" std::unique_ptr<Mapper<std::string, int, int, std::string>> Create() {
  return std::unique_ptr<Mapper<std::string, int, int, std::string>>(new WordCountMapper);
}


#endif //SLAVERDD_WORD_COUNT_MAPPER_H
