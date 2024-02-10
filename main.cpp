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
  int value1 = 5;
  int value4 = 7;
  int value3 = 6;
  int value = 3;

  // 5 7 6 3
  graph.add_node(value1);
  graph.add_node(value4);
  graph.add_node(value3);
  graph.add_node(value);

  graph.add_arc(value3, value1);
  graph.add_arc(value1, value3);
  graph.add_arc(value4, value3);
  graph.add_arc(value3, value4);
  graph.add_arc(value3, value3);

  std::cout << graph << std::endl;
  graph.delete_node(value1);

  std::cout << graph << std::endl;
  graph.delete_node(value);

  std::cout << graph << std::endl;
  graph.delete_node(value4);

  std::cout << graph << std::endl;
  graph.delete_node(value3);
  std::cout << graph << std::endl;

  return 0;
}
