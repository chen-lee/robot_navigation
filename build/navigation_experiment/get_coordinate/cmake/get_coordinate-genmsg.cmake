# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "get_coordinate: 1 messages, 0 services")

set(MSG_I_FLAGS "-Iget_coordinate:/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg;-Istd_msgs:/opt/ros/indigo/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(genlisp REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(get_coordinate_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg/position.msg" NAME_WE)
add_custom_target(_get_coordinate_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "get_coordinate" "/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg/position.msg" ""
)

#
#  langs = gencpp;genlisp;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(get_coordinate
  "/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg/position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/get_coordinate
)

### Generating Services

### Generating Module File
_generate_module_cpp(get_coordinate
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/get_coordinate
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(get_coordinate_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(get_coordinate_generate_messages get_coordinate_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg/position.msg" NAME_WE)
add_dependencies(get_coordinate_generate_messages_cpp _get_coordinate_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(get_coordinate_gencpp)
add_dependencies(get_coordinate_gencpp get_coordinate_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS get_coordinate_generate_messages_cpp)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(get_coordinate
  "/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg/position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/get_coordinate
)

### Generating Services

### Generating Module File
_generate_module_lisp(get_coordinate
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/get_coordinate
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(get_coordinate_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(get_coordinate_generate_messages get_coordinate_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg/position.msg" NAME_WE)
add_dependencies(get_coordinate_generate_messages_lisp _get_coordinate_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(get_coordinate_genlisp)
add_dependencies(get_coordinate_genlisp get_coordinate_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS get_coordinate_generate_messages_lisp)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(get_coordinate
  "/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg/position.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/get_coordinate
)

### Generating Services

### Generating Module File
_generate_module_py(get_coordinate
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/get_coordinate
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(get_coordinate_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(get_coordinate_generate_messages get_coordinate_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/chenlee/robot_navigation/src/navigation_experiment/get_coordinate/msg/position.msg" NAME_WE)
add_dependencies(get_coordinate_generate_messages_py _get_coordinate_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(get_coordinate_genpy)
add_dependencies(get_coordinate_genpy get_coordinate_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS get_coordinate_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/get_coordinate)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/get_coordinate
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
add_dependencies(get_coordinate_generate_messages_cpp std_msgs_generate_messages_cpp)

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/get_coordinate)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/get_coordinate
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
add_dependencies(get_coordinate_generate_messages_lisp std_msgs_generate_messages_lisp)

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/get_coordinate)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/get_coordinate\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/get_coordinate
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
add_dependencies(get_coordinate_generate_messages_py std_msgs_generate_messages_py)
