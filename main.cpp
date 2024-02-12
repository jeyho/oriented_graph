/**
@file main.cpp
@brief test d'uso della classe ordered_list templata
**/

// #include <iostream>
#include "oriented_graph.hpp"
#include <cassert>
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

void test_constructors() {

  // Costruzione di un grafo con tipi built-in
  // oriented_graph<std::string, equal_string> graph_string;
  // oriented_graph<int, equal_string> graph_int;

  // Altri test su tipi custom
}

void test_copyConstructor() {

  oriented_graph<int, equal_int> graph_int;

  // graph_int.add_node(12);
  oriented_graph<int, equal_int> graph_int_copy(graph_int);
  // assert(graph_int.nodes_number() == graph_int_copy.nodes_number());
  // assert(graph_int.arcs_number() == graph_int_copy.arcs_number());
  // assert(graph_int == graph_int_copy);

  graph_int_copy.add_node(10);
  graph_int_copy.delete_node(10);
  graph_int.delete_node(10);
  // std::cout << graph_int_copy << std::endl;
  // std::cout << graph_int << std::endl;

  // graph_int_copy.add_node(20);

  // assert(graph_int != graph_int_copy);

  // Altri test su tipi custom
}

int main(int argc, char *argv[]) {

  // test_constructors();
  test_copyConstructor();

  // oriented_graph<std::string, equal_string> graph;
  // std::string value1 = "5";
  // std::string value4 = "7";
  // std::string value3 = "6";
  // std::string value = "3";
  // std::string valuex = "8";
  //
  // // 5 7 6 3
  // graph.add_node(value1);
  // graph.add_node(value4);
  // graph.add_node(value3);
  // graph.add_node(value);
  //
  // graph.add_arc(value3, value1);
  // graph.add_arc(value1, value3);
  // graph.add_arc(value4, value3);
  // graph.add_arc(value3, value4);
  // graph.add_arc(value3, value3);
  // // graph.retrieve_index_id_node(valuex);
  //
  // oriented_graph<std::string, equal_string> graph2(graph);
  //
  // std::cout << graph << std::endl;
  // // std::cout << graph2 << std::endl;
  //
  // // testiamo iteratori
  // oriented_graph<std::string, equal_string>::const_iterator it_init;
  // oriented_graph<std::string, equal_string>::const_iterator it_end;
  //
  // it_end = graph.end();
  //
  // for (it_init = graph.begin(); it_init != it_end; ++it_init)
  //   std::cout << *it_init << std::endl;
  //
  // it_init = graph.begin();
  // std::cout << *it_init << std::endl;
  //
  // std::cout << "Numero di archi: " << graph.arcs_number() << std::endl;
  // std::cout<<*it_end<<std::endl;

  return 0;
}
