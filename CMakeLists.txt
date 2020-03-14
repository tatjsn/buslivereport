project(app CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)

add_executable(app
  domain/script.cc
  db.cc
  app.cc)

target_link_libraries(app ${CONAN_LIBS})
set_target_properties(app PROPERTIES CXX_STANDARD 17)
