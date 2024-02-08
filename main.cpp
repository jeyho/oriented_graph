/**
@file main.cpp
@brief test d'uso della classe ordered_list templata
**/

// #include <iostream>
// #include <cassert>
#include "oriented_graph.hpp"
#include <iostream>

/**
  @brief Funtore di uguaglianza tra tipi interi

  Valuta l'uglianza tra due interi.
*/
struct equal_int {
  bool operator()(int a, int b) const { return a == b; }
};

/**
  @brief Funtore per l'uguaglianza tra stringhe.

  Funtore per l'ugagianza tra stringhe. La valutazione e' fatta
  lessicografica.
*/
struct equal_string {
  bool operator()(const std::string &a, const std::string &b) const {
    return (a == b);
  }
};
int main(int argc, char *argv[]) {
  oriented_graph<int, equal_int> graph;
  int value = 3;
  int value1 = 5;
  int value3 = 6;
  int value4 = 7;
  graph.add_node(value1);
  graph.add_node(value4);
  oriented_graph<int, equal_int> graph2(graph);

  std::cout << graph << std::endl;
  std::cout << graph2 << std::endl;

  return 0;
}
