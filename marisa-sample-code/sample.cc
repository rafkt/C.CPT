// sample.cc
#include <iostream>
#include <marisa.h>
#include <string>

int main() {
  marisa::Keyset *keyset = new marisa::Keyset();
  // keyset.push_back("a");
  // keyset.push_back("app");
  // keyset.push_back("apple");
  std::string str = "fast3";
  keyset->push_back("fast1");
  keyset->push_back("fast2");
  marisa::Key key;
  key.set_str(str.c_str());
  keyset->push_back(key);
  // keyset.push_back("fa");
  // keyset.push_back("f");

  marisa::Trie *trie = new marisa::Trie();
  trie->build(*keyset, 1);

  std::cout << trie->io_size() << std::endl;

  marisa::Agent agent;
  agent.set_query("fast3");
  while (trie->common_prefix_search(agent)) {
    std::cout.write(agent.key().ptr(), agent.key().length());
    std::cout << ": " << agent.key().id() << std::endl;
  }
  return 0;
}