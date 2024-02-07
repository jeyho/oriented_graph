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
  oriented_graph<int, equal_string> graph;
  int value=3;
  int value1=5;
  int value3=6;
  graph.add_node(value);
  std::cout<<"creazione nodo 2"<<std::endl;
  graph.add_node(value1);
  graph.add_node(value3);
  graph.add_node(value);
  std::cout<<"fine creazione nodo 2"<<std::endl;
  std::cout<<graph;
  return 0;
}
