#include "algorithm"
#include "arc_already_exists.hpp"
#include "id_node_already_exists.hpp"
#include "oriented_graph.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

struct Car {
  std::string _brand;
  std::string _model;

  Car() : _brand(""), _model("") {}

  ~Car() {
    this->_brand = "";
    this->_model = "";
  }

  Car(const Car &other) : _brand(""), _model("") {
    this->_model = other._model;
    this->_brand = other._brand;
  }

  Car(const std::string &brand, const std::string &model)
      : _brand(""), _model("") {
    this->_brand = brand;
    this->_model = model;
  }
  void swap(Car &other) {
    std::swap(this->_model, other._model);
    std::swap(this->_brand, other._brand);
  }

  Car &operator=(Car &other) {
    if (this != &other) {
      Car tmp(other);
      swap(tmp);
    }
    return *this;
  }

  bool operator==(const Car &other) const {
    return (_brand == other._brand && _model == other._model);
  }
  std::string value_brand() const { return _brand; }
  std::string value_model() const { return _model; }
};

/**
 * @brief Funtore di uguaglianza tra dati di tipo Car.
 */
struct Car_equal {
  bool operator()(const Car &a, const Car &b) const { return (a == b); }
};

/**
 *@brief Funtore per l'uguaglianza tra due numeri interi
 */
struct equal_int {
  bool operator()(int a, int b) const { return a == b; }
};

/**
 *@brief Funtore per l'uguaglianza tra due stringhe
 */
struct equal_string {
  bool operator()(std::string a, std::string b) const { return (a == b); }
};

/**
 * @brief Test struct car
 */
void test_car() {
  // Test default constructor
  Car car;
  assert(car.value_brand() == "" && car.value_model() == "");

  // Test secundary constructor
  Car car2("Ford", "Mustang");
  assert(car2.value_brand() == "Ford" && car2.value_model() == "Mustang");

  // Test copy constructor
  Car car3(car2);
  assert(car2.value_brand() == car3.value_brand() &&
         car2.value_model() == car3.value_model());

  // Test ridefinizione operatore ==
  assert(car2 == car3);

  // Test assegnamento
  car2 = car;
  assert(car2.value_brand() == "" && car2.value_model() == "");
}

/**
 * @brief Test costruttore di default
 */
void test_default_constructor() {
  oriented_graph<int, equal_int> graph;
  assert(graph.size() == 0);
  assert(graph.nodes_number() == 0);
  assert(graph.arcs_number() == 0);
}

/**
 * @brief Test aggiunta nodo
 */
void test_add_node() {
  oriented_graph<int, equal_int> graph;
  graph.add_node(1);
  graph.add_node(2);
  graph.add_node(3);
  assert(graph.nodes_number() == 3);
  assert(graph.existsNode(1));
  assert(graph.existsNode(2));
  assert(graph.existsNode(3));

  try {
    // graph.add_node(1);
  } catch (const id_node_already_exists &e) {
    std::cout << e.what() << std::endl;
  }
}

/**
 * @brief Test rimozione nodo
 */
void test_remove_nodo() {
  oriented_graph<int, equal_int> graph;
  graph.add_node(1);
  graph.add_node(2);

  graph.remove_node(2);
  assert(graph.nodes_number() == 1);
  assert(graph.existsNode(1));
  assert(!graph.existsNode(2));
}

/**
 * @brief Test rimozione arco
 */
void test_remove_arc() {}

/**
 * @brief Test aggiunta arco
 */
void test_add_arc() {
  oriented_graph<int, equal_int> graph;
  graph.add_node(1);
  graph.add_node(2);
  graph.add_node(3);

  graph.add_arc(1, 1);
  graph.add_arc(1, 3);
  graph.add_arc(3, 1);
  graph.add_arc(3, 3);
  graph.add_arc(2, 2);

  assert(graph.arcs_number() == 5);
  assert(graph.existsEdge(1, 1));
  assert(graph.existsEdge(1, 3));
  assert(graph.existsEdge(3, 1));
  assert(graph.existsEdge(3, 3));
  assert(graph.existsEdge(2, 2));

  try {
    // graph.add_arc(1, 1);
  } catch (const arc_already_exists &e) {
    std::cout << e.what() << std::endl;
  }
}

/**
 * @brief Test aggiunta arco
 */
void test_copy_constructor() {
  oriented_graph<int, equal_int> graph;
  oriented_graph<int, equal_int> graph_copy(graph);
  graph_copy.add_node(1);

  // continuare da qui
}

int main(int argc, char *argv[]) {

  // test_default_constructor();
  test_car();
  test_default_constructor();
  test_copy_constructor();
  test_add_node();
  test_add_arc();
  test_remove_nodo();

  return 0;
}
