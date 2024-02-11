/**
@file main.cpp
@brief test d'uso della classe ordered_list templata
**/

// #include <iostream>
// #include <cassert>
#include "oriented_graph.hpp"
#include <iostream>
#include <ostream>
#include <string>

/**
  @brief Funtore di uguaglianza tra tipi interi

  Valuta l'uglianza tra due interi.
*/
struct equal_int {
  bool operator()(int a, int b) const { return a == b; }
};

/**
  @brief Funtore per l'uguaglianza tra stringhe.

  Funtore per l'ugagianza tra stringhe.
*/
struct equal_string {
  bool operator()(std::string a, std::string b) const { return (a == b); }
};

int main(int argc, char *argv[]) {
  oriented_graph<std::string, equal_string> graph;
  std::string value1 = "5";
  std::string value4 = "7";
  std::string value3 = "6";
  std::string value = "3";
  std::string valuex = "8";

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

  oriented_graph<std::string, equal_string> graph2(graph);

  std::cout << graph << std::endl;
  // std::cout << graph2 << std::endl;

  // testiamo iteratori
  oriented_graph<std::string, equal_string>::const_iterator it_init;
  oriented_graph<std::string, equal_string>::const_iterator it_end;

  it_end = graph.end();

  for (it_init = graph.begin(); it_init != it_end; ++it_init)
    std::cout << *it_init << std::endl;

  it_init = graph.begin();
  std::cout << *it_init << std::endl;
  // std::cout<<*it_end<<std::endl;

  return 0;
}
