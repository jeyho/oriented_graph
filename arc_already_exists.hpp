#ifndef ARC_ALREADY_EXISTS
#define ARC_ALREADY_EXISTS

#include <stdexcept>
#include <string>

/**

 * @author 844895 Rojas John
 * @file id_node_already_exists.hpp
 **/

class arc_already_exists : public std::runtime_error {
public:
  /**
   * @brief Constructor, accepts a std::string
   * @param message, the error to be displayed (std::string type)
   */
  arc_already_exists(const std::string &message)
      : std::runtime_error(message) {}
};

#endif
