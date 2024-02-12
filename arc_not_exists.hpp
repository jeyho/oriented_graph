#ifndef ARC_NOT_EXISTS
#define ARC_NOT_EXISTS

#include <stdexcept>
#include <string>

/**

 * @author 844895 Rojas John
 * @file id_node_already_exists.hpp
 **/

class arc_not_exists : public std::logic_error {
public:
  /**
   * @brief Constructor, accepts a std::string
   * @param message, the error to be displayed (std::string type)
   */
  arc_not_exists(const std::string &message) : std::logic_error(message) {}
};

#endif
