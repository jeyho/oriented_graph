/**
  @file oriented_graph.hpp

  @brief File header della classe oriented_graph templata

  File contenente le dichiarazioni/definizioni della classe oriented_graph
  templata
*/
#ifndef ORIENTED_GRAPH
#define ORIENTED_GRAPH
#include <algorithm>
#include <iostream>
#include <iterator>

template <typename T, typename Eql> class oriented_graph {
public:
  typedef unsigned int size_type;
  /**
      Costruttore di default

      @post _adj_matrix == nullptr
      @post _value_set == nullptr
    */
  oriented_graph() : _adj_matrix(nullptr), _value_set(nullptr), _size(0) {

#ifndef NDEBUG
    std::cout << "oriented_graph()" << std::endl;
#endif // !NDEBUG
  }

  /**
      Copy constructor

      @param other grafo orientato da copiare
  */
  oriented_graph(const oriented_graph &other)
      : _adj_matrix(nullptr), _value_set(nullptr), _size(0) {

#ifndef NDEBUG
    std::cout << "oriented_graph(const oriented_graph &other)" << std::endl;
#endif // !NDEBUG
  }

  /**
        Operatore assegnamento

        @param other grafo orientato da copiare
    */
  oriented_graph &operator=(const oriented_graph &other) {
#ifndef NDEBUG
    std::cout << " oriented_graph &operator=(...)" << std::endl;
#endif // !NDEBUG
  }

  // delete
  ~oriented_graph() {
    clean_matrix(_adj_matrix, _size);
    delete[] _value_set;
    _value_set = nullptr;
#ifndef NDEBUG
    std::cout << "~oriented_graph() " << std::endl;
#endif // !NDEBUG
  }

  // Metodo richiesto dal progetto
  void add_node(const T &value) {
    // caso in cui numero nodi = 0
    if (_size == 0) {
      // creazione matrice
      _adj_matrix = create_matrix(_adj_matrix, _size + 1);
      fill_matrix(_adj_matrix, _size + 1, false);

      // aggiungiamo identificativo  del nodo al set di valori
      _value_set = new T[_size + 1];
      _value_set[_size + 1] = value;

    } else {
      // aggiorniamo la capacità della matrice
      update_matrix_capacity(_adj_matrix, _size);

      // aggiungiamo identificativo del nodo al set di valori
      update_set_capacity(_value_set, _size);
      _value_set[_size + 1] = value;
    }
    // aggiornimao numero dei nodi (dimensione matrice)
    _size = _size + 1;
#ifndef NDEBUG
    std::cout << "void add_node(...)" << std::endl;
#endif
  }

  bool existsNode(const T &value) const {
    size_type index = 0;
    while (index < _size) {
      if (_equal(_value_set[index], value))
        return true;
    }
    return false;
#ifndef NDEBUG
    std::cout << "bool existsNode(...)"  << std::endl;
#endif
  }

  /**
   * Questo metodo aumenta la capacità di value_set contenente gli
   * identificatori dei nodi da una dimensione n a n+1.
   *
   * @param value_set Un riferimento a un puntatore a un oggetto di tipo T
   * contenente gli identificatori dei nodi.
   *
   * @param size La dimensione corrente del set.
   *
   * Precondizioni: value_set deve essere stato istanziato in precedenza
   */
  void update_set_capacity(T *&value_set, const size_type size) {
    // Allocazione di memoria per un nuovo array di dimensione size + 1
    T *tmp = new T[size + 1];

    // Copiamo i dati di value_set all'interno di tmp
    for (size_type i = 0; i < size; ++i) {
      tmp[i] = value_set[i];
    }

    // Deallocazione della memoria occupata dall'array originale
    delete[] value_set;
    value_set = nullptr;

    // Assegnazione del puntatore value_set alla memoria dinamica allocata per
    // tmp
    value_set = tmp;
    tmp = nullptr;
#ifndef NDEBUG
    std::cout << "void update_set_capacity(...) " << std::endl;
#endif
  }

  /**
   * Aumenta la capacità della matrice matrix da una dimensione n a una
   * dimesione n+1. La matrice aggiorna la dimensione mantentendo intatti i dati
   * al suo interno
   *
   * @param matrix Un riferimento a un puntatore a puntatore a bool.
   *
   * @param size La dimensione di matrix inziale
   */
  void update_matrix_capacity(bool **&matrix, const size_type size) {
    bool **tmp = create_matrix(tmp, size + 1);
    fill_matrix(tmp, size + 1, false);
    copy_matrix_values(matrix, tmp, size);
    clean_matrix(matrix, size);
    matrix = tmp;
    tmp = nullptr;
#ifndef NDEBUG
    std::cout << "void update_matrix(...)" << std::endl;
#endif
  }

  /**
    Precondizioni:
    1) Si presume che la matrice destination_matrix e source_matrix siano già
    state istanziate;

    2) Sia n la dimensione di  destination_matrix, si presume
    che la source_matrix abbia come dimensione massima al più n-1
  */
  void copy_matrix_values(bool **source_matrix, bool **destination_matrix,
                          const size_type source_matrix_size) {
    for (size_type i = 0; i < source_matrix_size; ++i) {
      for (size_type j = 0; j < source_matrix_size; ++j) {
        destination_matrix[i][j] = source_matrix[i][j];
      }
    }
#ifndef NDEBUG
    std::cout << "void copy_matrix_values(...)" << std::endl;
#endif
  }

  bool **create_matrix(bool **matrix, const size_type size) {
    // creo le righe
    matrix = new bool *[size];

    // creo le colonne
    for (size_type i = 0; i < size; ++i)
      matrix[i] = new bool[size];

#ifndef NDEBUG
    std::cout << "void create_matrix(...)" << std::endl;
#endif
    return matrix;
  }

  void clean_matrix(bool **&matrix, const size_type size) {
    for (size_type i = 0; i < size; ++i)
      delete[] matrix[i];

    delete[] matrix;
    matrix = nullptr;

#ifndef NDEBUG
    std::cout << "void delete_matrix(...)" << std::endl;
#endif
  }

  void fill_matrix(bool **matrix, const size_type size, const bool value) {
    for (size_type i = 0; i < size; ++i) {
      for (size_type j = 0; j < size; ++j) {
        matrix[i][j] = value;
      }
    }
#ifndef NDEBUG
    std::cout << "void fill_matrix(...)" << std::endl;
#endif
  }

  int size() const { return _size; }

  template <typename U, typename V>
  friend std::ostream &operator<<(std::ostream &os,
                                  const oriented_graph<U, V> &graph);
  // friend std::ostream &operator<<(std::ostream &os,
  //                                 const oriented_graph<T, Eql> &graph);
  //
private:
  bool **_adj_matrix;
  T *_value_set;
  size_type _size;
  Eql _equal;

  // dovremmo inserire la lista degli archi?
  // da inserire possibili funtori
};

template <typename T, typename Eql>
std::ostream &operator<<(std::ostream &os,
                         const oriented_graph<T, Eql> &graph) {
  // mostriamo la matrice
  os << "Dimensione: " << graph._size << std::endl;
  for (typename oriented_graph<T, Eql>::size_type i = 0; i < graph.size();
       ++i) {
    for (typename oriented_graph<T, Eql>::size_type j = 0; j < graph.size();
         ++j) {
      os << graph._adj_matrix[i][j];
    }
    os << std::endl;
  }

  // // mostriamo il set di nodi
  // for (typename oriented_graph<T, Eql>::size_type i = 0; i < graph.size();
  //      ++i) {
  //   os << graph._value_set[i];
  // }
  return os;
}
#endif
