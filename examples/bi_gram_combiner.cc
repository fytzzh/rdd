//
// Created by Harunobu Daikoku on 2015/12/10.
//

#include <numeric>
#include "bi_gram_combiner.h"

std::pair<std::pair<std::string, std::string>, int> BiGramCombiner::Reduce(const std::pair<std::string,
                                                                                           std::string> &key,
                                                                           const std::vector<int> &values) {
  return std::make_pair(key, std::accumulate(values.begin(), values.end(), 0));
}