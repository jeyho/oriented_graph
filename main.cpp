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
  oriented_graph<int, equal_int> graph2(graph);
  graph2.add_node(value3);
  graph2.add_node(value);
  graph2.add_arc(value3, value1);
  graph2.add_arc(value1, value3);
  graph2.add_arc(value4, value3);
  graph2.delete_node(value1);
  graph2.delete_node(value);
  graph2.delete_node(value4);
  // la delete funziona ha problemi con quelli di dimensione due ma il resto
  // sembra andare controllare e migliorare il codice relativo alla delete, al
  // momento funziona!
  std::cout << graph2 << std::endl;

  return 0;
}
