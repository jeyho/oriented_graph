/**
  @file oriented_graph.hpp

  @brief File header della classe oriented_graph templata

  File contenente le dichiarazioni/definizioni della classe oriented_graph
  templata
*/

#ifndef ORIENTED_GRAPH
#define ORIENTED_GRAPH
#include "arc_already_exists.hpp"
#include "arc_not_exists.hpp"
#include "id_node_already_exists.hpp"
#include "id_node_not_exists.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <strings.h>

/**
 * @brief Classe oriented_graph
 * @param init the initial value to fill the cube with
 * @pre (z > 0 && y > 0 && x > 0)
 * @post cube != nullptr
 * @throw bad_build_size custom exception throwed if pages, row, column size
 * is <= 0
 * @throw std::exception possible exception coming from generic type T
 * @throw std::bad_alloc possible allocation error coming from new[]
 */

template <typename T, typename Eql> class oriented_graph {
public:
  typedef unsigned int size_type;

  /**
   * @brief Classe oriented_graph
   * @param init the initial value to fill the cube with
   * @pre (z > 0 && y > 0 && x > 0)
   * @post cube != nullptr
   * @throw bad_build_size custom exception throwed if pages, row, column size
   * is <= 0
   * @throw std::exception possible exception coming from generic type T
   * @throw std::bad_alloc possible allocation error coming from new[]
   */

  oriented_graph() : _adj_matrix(nullptr), _id_buffer(nullptr), _size(0) {}

  /**
      Copy constructor

      @param other grafo orientato da copiare
  */
  oriented_graph(const oriented_graph &other)
      : _adj_matrix(nullptr), _id_buffer(nullptr), _size(0) {
    _id_buffer = new T[other._size];
    try {
      for (size_type i = 0; i < other._size; ++i) {
        _id_buffer[i] = other._id_buffer[i];
      }
      _adj_matrix = create_matrix(_adj_matrix, other._size);
    } catch (...) {
      clean_id_buffer(_id_buffer);
      clean_matrix(_adj_matrix, _size);
      throw;
    }
    copy_matrix_values(other._adj_matrix, _adj_matrix, other._size);
    _size = other._size;
  }

  size_type nodes_number() const { return _size; }

  size_type arcs_number() const {
    int counter = 0;
    for (size_type i = 0; i < _size; ++i)
      for (size_type j = 0; j < _size; ++j) {
        if (_adj_matrix[i][j] == true)
          ++counter;
      }
    return counter;
  }

  /**
        Operatore assegnamento

        @param other grafo orientato da copiare
    */
  oriented_graph &operator=(const oriented_graph &other) {
    if (this != &other) {
      oriented_graph tmp(other);
      this->swap(tmp);
    }
    return *this;
  }

  bool operator==(const oriented_graph &other) const {
    if (this->_size != other._size)
      return false;

    if (this->arcs_number() != other.arcs_number())
      return false;

    for (size_type i = 0; i < _size; ++i) {
      if (!_equal(_id_buffer[i], other._id_buffer[i]))
        return false;
    }
    for (size_type i = 0; i < _size; ++i) {
      for (size_type j = 0; i < _size; ++j)
        if (!_equal(_adj_matrix[i][j], other._adj_matrix[i][j]))
          return false;
    }

    return true;
  }

  // bool operator!=(const oriented_graph &other) const {
  //   return !(*this == other);
  // }

  void swap(oriented_graph &other) {
    std::swap(this->_size, other._size);
    std::swap(this->_adj_matrix, other._adj_matrix);
    std::swap(this->_id_buffer, other._id_buffer);
    std::swap(this->_equal, other._equal);
  }

  // delete
  ~oriented_graph() { clean(); }
  void clean() {
    clean_matrix(_adj_matrix, _size);
    clean_id_buffer(_id_buffer);
    _size = 0;
  }

  // Metodo richiesto dal progetto
  void add_node(const T &value) {
    if (existsNode(value)) {
      throw id_node_already_exists(
          "Error, the id already exists inside the buffer");
    }
    try {
      update_adj_matrix();
      update_id_buffer(value);
    } catch (...) {
      clean();
      throw;
    }
    _size = _size + 1;
  }

  void remove_node(const T &value) {
    if (!existsNode(value)) {
      throw id_node_not_exists(
          "Error, the id node not exists inside the buffer");
    }
    size_type index_to_delete = retrieve_index_id_node(value);
    oriented_graph tmp;
    try {
      for (size_type i = 0; i < _size; ++i) {
        if (index_to_delete != i)
          tmp.add_node(_id_buffer[i]);
      }
    } catch (...) {
      clean();
    }
    copy_matrix_with_different_sizes(_adj_matrix, tmp._adj_matrix, _size,
                                     tmp._size, index_to_delete);
    *this = tmp;
  }

  /**
   * @brief Secondary constructor
   * @param init the initial value to fill the cube with
   * @pre (z > 0 && y > 0 && x > 0)
   * @post cube != nullptr
   * @throw bad_build_size custom exception throwed if pages, row, column size
   * is <= 0
   * @throw std::exception possible exception coming from generic type T
   * @throw std::bad_alloc possible allocation error coming from new[]
   */
  void add_arc(const T &value_start, const T &value_destination) {
    if (existsEdge(value_start, value_destination)) {
      throw arc_already_exists("Error, the arc already exists in the graph!");
    }
    int index_value_start = retrieve_index_id_node(value_start);
    int index_value_destination = retrieve_index_id_node(value_destination);
    _adj_matrix[index_value_start][index_value_destination] = true;
  }

  void remove_arc(const T &value_start, const T &value_destination) {
    assert(existsNode(value_start) && existsNode(value_destination));
    if (!existsEdge(value_start, value_destination)) {
      throw arc_not_exists("Error, the arc doesn't exists in the graph!");
    }
    int index_value_start = retrieve_index_id_node(value_start);
    int index_value_destination = retrieve_index_id_node(value_destination);
    _adj_matrix[index_value_start][index_value_destination] = false;
  }

  // Metodo chiesto dal prof
  bool existsNode(const T &value) const {
    size_type index = 0;
    while (index < _size) {
      if (_equal(_id_buffer[index], value))
        return true;
      ++index;
    }
    return false;
  }

  // assumo che i nodi id_start_node e id_end_node esistano nel grafo
  bool existsEdge(const T &id_start_node, const T &id_end_node) const {
    assert(existsNode(id_start_node) && existsNode(id_end_node));
    size_type index_id_start_node = retrieve_index_id_node(id_start_node);
    size_type index_id_end_node = retrieve_index_id_node(id_end_node);
    return (_adj_matrix[index_id_start_node][index_id_end_node]) ? true : false;
    // if (_adj_matrix[index_id_start_node][index_id_end_node]) {
    //   return true;
    // }
    // return false;
  }

  /*
   * la seguente funzione copia i valori dalla matrice di adiacenza di origine
   * alla matrice di adiacenza di destinazione in seguito a una cancellazione di
   * un nodo la cui posizione all'interno della matrice viene passato tramite
   * index_value_to_delete. La funzione gestisce dimensioni diverse per le due
   * matrici.
   *
   * @param matrix_adj_source La matrice di adiacenza di origine da cui copiare
   * i valori.
   *
   * @param matrix_adj_destination La matrice di adiacenza di destinazione in
   * cui inserire i valori copiati.
   *
   * @param source_size La dimensione della matrice di adiacenza di origine.
   *
   * @param destination_size La dimensione della matrice di adiacenza di
   * destinazione.
   *
   * @param index_to_delete L'indice della riga e della colonna da eliminare
   * dalla copia.
   *
   * Pre condizioni Come pre-condizione, si presume che entrambi
   * siano già stati istanziati in precedenza Si presume che matrix_adj_source
   * abbia dimensione n Si presume che matrix_adj_destination abbia  dimensione
   * n-1
   *
   * */
  void copy_matrix_with_different_sizes(bool **matrix_adj_source,
                                        bool **matrix_adj_destination,
                                        const size_type source_size,
                                        const size_type destination_size,
                                        size_type index_value_to_delete) {
    size_type row_destination = 0;
    size_type colum_destination = 0;
    for (size_type row_source = 0; row_source < source_size; ++row_source) {
      for (size_type colum_source = 0; colum_source < source_size;
           ++colum_source) {
        if (row_source != index_value_to_delete &&
            colum_source != index_value_to_delete) {
          matrix_adj_destination[row_destination][colum_destination] =
              matrix_adj_source[row_source][colum_source];
          ++colum_destination;
        }
      }
      if (colum_destination >= destination_size) {
        ++row_destination;
        colum_destination = 0;
      } // in questa parte gestire il caso
    }
  }

  /*
   *Si presume che il nodo sia presente nel buffer
   *
   *
   * */
  const size_type retrieve_index_id_node(const T &value) const {
    // assumiamo che il nodo sia nella lista! va bene la assert in questo caso?
    assert(existsNode(value));
    size_type index = 0;
    while (index < _size) {
      if (_equal(_id_buffer[index], value))
        return index;
      ++index;
    }
    return index;
  }

  /**
   * Questo metodo aumenta la capacità di  contenente gli
   * identificatori dei nodi da una dimensione n a n+1 inserendo un
   * nuovo identificativo all'interno di id_buffer.
   *
   * @param value identificativo del nodo da aggiungere a id_buffer
   */
  void update_id_buffer(const T &value) {
    T *tmp = new T[_size + 1];
    for (size_type i = 0; i < _size; ++i) {
      tmp[i] = _id_buffer[i];
    }
    clean_id_buffer(_id_buffer);
    _id_buffer = tmp;
    _id_buffer[_size] = value;
    tmp = nullptr;
  }

  /**
   * Aumenta la capacità della matrice matrix da una dimensione n a una
   * dimesione n+1. La matrice aggiorna la dimensione mantentendo intatti i dati
   * al suo interno
   */
  void update_adj_matrix() {
    bool **tmp = create_matrix(tmp, _size + 1);
    fill_matrix(tmp, _size + 1, false);
    copy_matrix_values(_adj_matrix, tmp, _size);
    clean_matrix(_adj_matrix, _size);
    _adj_matrix = tmp;
    tmp = nullptr;
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
  }

  bool **create_matrix(bool **matrix, const size_type size) {
    // creo le righe
    matrix = new bool *[size];

    // creo le colonne
    for (size_type i = 0; i < size; ++i)
      matrix[i] = new bool[size];
    return matrix;
  }

  void clean_matrix(bool **&matrix, const size_type size) {
    for (size_type i = 0; i < size; ++i)
      delete[] matrix[i];

    delete[] matrix;
    matrix = nullptr;
  }

  void clean_id_buffer(T *&id_buffer) {
    delete[] id_buffer;
    id_buffer = nullptr;
  }

  void fill_matrix(bool **matrix, const size_type size, const bool value) {
    for (size_type i = 0; i < size; ++i) {
      for (size_type j = 0; j < size; ++j) {
        matrix[i][j] = value;
      }
    }
  }

  int size() const { return _size; }

  template <typename U, typename V>
  friend std::ostream &operator<<(std::ostream &os,
                                  const oriented_graph<U, V> &graph);

  class const_iterator {
    // { }
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T *pointer;
    typedef const T &reference;

    const_iterator() : _ptr(nullptr) {}

    const_iterator(const const_iterator &other) : _ptr(other._ptr) {}

    const_iterator &operator=(const const_iterator &other) {
      _ptr = other._ptr;
      return *this;
    }

    ~const_iterator() {}

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const { return *_ptr; }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const { return _ptr; }

    // Operatore di iterazione post-incremento
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++_ptr;
      return tmp;
    }

    // Operatore di iterazione pre-incremento
    const_iterator &operator++() {
      ++_ptr;
      return *this;
    }

    // Uguaglianza
    bool operator==(const const_iterator &other) const {
      return _ptr == other._ptr;
    }

    // Diversita'
    bool operator!=(const const_iterator &other) const {
      return _ptr != other._ptr;
    }

  private:
    // Dati membro

    const T *_ptr;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class oriented_graph; // !!! Da cambiare il nome!

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    const_iterator(const T *p) : _ptr(p) {}

  }; // classe const_iterator

  /**
   * @brief Funzione che ritorna l'iteratore all'inizio della sequenza degli
   * identificatori dei nodi
   *
   * @return const_iterator all'inizio della sequenza degli identificatori dei
   * nodi
   */
  const_iterator begin() const { return const_iterator(_id_buffer); }

  /**
   * @brief Funzione che ritorna l'iteratore alla fine della sequenza degli
   * identificatori dei nodi
   *
   * @return const_iterator alla fine della sequenza degli identificatori dei
   * nodi
   */
  const_iterator end() const { return const_iterator(_id_buffer + _size); }

private:
  bool **_adj_matrix;
  T *_id_buffer;
  size_type _size;
  Eql _equal;
};

/**
 * @brief Ridefinizione dell'operatore di stream per l'invio del contenuto di
 * oriented_graph sullo stream in output
 *
 * @param os Stream di output
 * @param graph oriented_graph da inviare
 * @return Reference allo stream di output
 */
template <typename T, typename Eql>
std::ostream &operator<<(std::ostream &os,
                         const oriented_graph<T, Eql> &graph) {
  os << "Size: " << graph._size << std::endl;
  for (typename oriented_graph<T, Eql>::size_type i = 0; i < graph.size();
       ++i) {
    for (typename oriented_graph<T, Eql>::size_type j = 0; j < graph.size();
         ++j) {
      os << graph._adj_matrix[i][j] << " ";
    }
    os << std::endl;
  }
  os << "Nodes: " << std::endl;
  for (typename oriented_graph<T, Eql>::size_type i = 0; i < graph.size();
       ++i) {
    os << graph._id_buffer[i] << " ";
  }

  return os;
}
#endif
