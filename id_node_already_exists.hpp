#ifndef ID_NODE_ALREADY_EXISTS
#define ID_NODE_ALREADY_EXISTS

#include <stdexcept>
#include <string>

/**

 * @author 844895 Rojas John
 * @file id_node_already_exists.hpp
 **/

class id_node_already_exists : public std::logic_error {
public:
  /**
   * @brief Constructor, accepts a std::string
   * @param message, the error to be displayed (std::string type)
   */
  id_node_already_exists(const std::string &message)
      : std::logic_error(message) {}
};

#endif
