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

    _value_set = new T[other._size];
    try {
      for (size_type i = 0; i < other._size; ++i) {
        _value_set[i] = other._value_set[i];
      }
      _adj_matrix = create_matrix(_adj_matrix, other._size);
      copy_matrix_values(other._adj_matrix, _adj_matrix, other._size);

    } catch (...) {
      clean_set(_value_set);
      clean_matrix(_adj_matrix, _size);
      throw;
    }
    _size = other._size;

#ifndef NDEBUG
    std::cout << "oriented_graph(const oriented_graph &other)" << std::endl;
#endif // !NDEBUG
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
#ifndef NDEBUG
    std::cout << " oriented_graph &operator=(...)" << std::endl;
#endif
    return *this;
  }

  void swap(oriented_graph &other) {
    std::swap(this->_size, other._size);
    std::swap(this->_adj_matrix, other._adj_matrix);
    std::swap(this->_value_set, other._value_set);
    std::swap(this->_equal, other._equal);
  }

  // delete
  ~oriented_graph() {
    clean_matrix(_adj_matrix, _size);
    clean_set(_value_set);
#ifndef NDEBUG
    std::cout << "~oriented_graph() " << std::endl;
#endif // !NDEBUG
  }

  // Metodo richiesto dal progetto
  void add_node(const T &value) {
    if (existsNode(value)) {
      throw id_node_already_exists(
          "Error, the id node already exists inside the set");
    }
    // aggiorniamo la capacità della matrice
    update_adj_matrix();

    // aggiungiamo identificativo al set di valori
    update_set(value);
    // aggiornimao numero dei nodi (dimensione matrice)
    _size = _size + 1;
#ifndef NDEBUG
    std::cout << "void add_node(...)" << std::endl;
#endif
  }

  void delete_node(const T &value) {
    if (!existsNode(value)) {
      throw id_node_not_exists("Error, the id node not exists inside the set");
    }
    size_type index_to_delete = retrieve_index_id_node(value);
    // caso in cui abbiamo un solo elemento
    if (_size == 1) {
      clean_matrix(_adj_matrix, _size);
      clean_set(_value_set);
      _size = _size - 1;
    } else {

      // caso in cui abbiamo più elementi
      oriented_graph<T, Eql> tmp;
      for (size_type i = 0; i < _size; ++i) {
        if (index_to_delete != i)
          tmp.add_node(_value_set[i]);
      }
      copy_matrix_with_different_sizes(_adj_matrix, tmp._adj_matrix, _size,
                                       tmp._size, index_to_delete);
      *this = tmp;
    }
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
#ifndef NDEBUG
    std::cout << "void add_arc(...)" << std::endl;
#endif
  }

  void remove_arc(const T &value_start, const T &value_destination) {
    if (!existsEdge(value_start, value_destination)) {
      throw arc_not_exists("Error, the arc doesn't exists in the graph!");
    }
    int index_value_start = retrieve_index_id_node(value_start);
    int index_value_destination = retrieve_index_id_node(value_destination);
    _adj_matrix[index_value_start][index_value_destination] = false;
#ifndef NDEBUG
    std::cout << "void delete_arc(...)" << std::endl;
#endif
  }

  // Metodo chiesto dal prof
  bool existsNode(const T &value) const {
    size_type index = 0;
    while (index < _size) {
      if (_equal(_value_set[index], value))
        return true;
      ++index;
    }
#ifndef NDEBUG
    std::cout << "bool existsNode(...)" << std::endl;
#endif
    return false;
  }

  // assumo che i nodi id_start_node e id_end_node esistano nel grafo
  bool existsEdge(const T &id_start_node, const T &id_end_node) const {
    assert(existsNode(id_start_node) && existsNode(id_end_node));
    size_type index_id_start_node = retrieve_index_id_node(id_start_node);
    size_type index_id_end_node = retrieve_index_id_node(id_end_node);
    if (_adj_matrix[index_id_start_node][index_id_end_node]) {
      return true;
    } else {
#ifndef NDEBUG
      std::cout << "bool existsEdge(...)" << std::endl;
#endif
      return false;
    }
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
            colum_source != index_value_to_delete &&
            row_destination < destination_size &&
            colum_destination < destination_size) {

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
#ifndef NDEBUG
    std::cout << "void copy_matrix_with_different_sizes()" << std::endl;
#endif
  }

  // operatore che ritorna un elemento constante nella posizione index di
  // _value_set
  // valutare se mantenerlo constante o meno il tipo di ritorno
  // questa funzione non modifica lo stato della classe perciò lo specifichiamo
  // con la const in fondo
  // Non so se mantenre questa funzione,effettivamente non viene utilizzata al
  // momento
  const T &operator[](const size_type index) const {
    assert(index < _size);
    return *(_value_set + index);
  }

  /*
   *Si presume che il nodo sia presente nel set
   *
   *
   * */
  const size_type retrieve_index_id_node(const T &value) const {
    // assumiamo che il nodo sia nella lista! va bene la assert in questo caso?
    assert(existsNode(value));
    size_type index = 0;
    while (index < _size) {
      if (_equal(_value_set[index], value))
        return index;
      ++index;
    }
#ifndef NDEBUG
    std::cout << "const int retrieve_index_id_node(...)" << std::endl;
#endif
    return index;
  }

  /**
   * Questo metodo aumenta la capacità di value_set contenente gli
   * identificatori dei nodi da una dimensione n a n+1 inserendo un
   * nuovo identificativo all'interno di _value_set.
   *
   * @param value identificativo del nodo da aggiungere a _value_set
   */
  void update_set(const T &value) {
    // caso in cui il numero di nodi sia a zero
    if (_size == 0) {
      _value_set = new T[_size + 1];
      _value_set[_size] = value;
    } else {
      // Allocazione di memoria per un nuovo array di dimensione size + 1
      T *tmp = new T[_size + 1];

      // Copiamo i dati di value_set all'interno di tmp
      for (size_type i = 0; i < _size; ++i) {
        tmp[i] = _value_set[i];
      }
      // Deallocazione della memoria occupata dall'array originale
      clean_set(_value_set);
      _value_set = tmp;
      _value_set[_size] = value;
      // tmp = nullptr;
    }
#ifndef NDEBUG
    std::cout << "void update_set_capacity(...) " << std::endl;
#endif
  }

  /**
   * Aumenta la capacità della matrice matrix da una dimensione n a una
   * dimesione n+1. La matrice aggiorna la dimensione mantentendo intatti i dati
   * al suo interno
   */
  void update_adj_matrix() {
    if (_size == 0) {
      _adj_matrix = create_matrix(_adj_matrix, _size + 1);
      fill_matrix(_adj_matrix, _size + 1, false);
    } else {
      bool **tmp = create_matrix(tmp, _size + 1);
      fill_matrix(tmp, _size + 1, false);
      copy_matrix_values(_adj_matrix, tmp, _size);
      clean_matrix(_adj_matrix, _size);
      _adj_matrix = tmp;
    }
    // tmp = nullptr;
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

  void clean_set(T *&set) {
    delete[] set;
    set = nullptr;
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

    // !!! Eventuali altri metodi privati

  }; // classe const_iterator

  // Ritorna l'iteratore all'inizio della sequenza dati
  const_iterator begin() const { return const_iterator(_value_set); }

  // Ritorna l'iteratore alla fine della sequenza dati
  const_iterator end() const { return const_iterator(_value_set + _size); }

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
  // Stampa della dimensione
  os << "Dimensione: " << graph._size << std::endl;

  // Stampa della matrice
  for (typename oriented_graph<T, Eql>::size_type i = 0; i < graph.size();
       ++i) {
    for (typename oriented_graph<T, Eql>::size_type j = 0; j < graph.size();
         ++j) {
      os << graph._adj_matrix[i][j] << " ";
    }
    os << std::endl;
  }

  // Stampa del set di nodi
  os << "Insieme dei nodi: " << std::endl;
  for (typename oriented_graph<T, Eql>::size_type i = 0; i < graph.size();
       ++i) {
    os << graph._value_set[i] << " ";
  }

  return os;
}
#endif
