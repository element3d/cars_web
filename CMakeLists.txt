CMAKE_MINIMUM_REQUIRED(VERSION 3.13 FATAL_ERROR)
PROJECT("cars_web")
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

# Set the binary directory to a specific path
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

# Specify the output directory for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(third_party/cpr)

add_subdirectory(libwebp)
#find_package(WebP)


set (LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
include_directories(${LIBS_DIR}/pqxx/include)
include_directories(${LIBS_DIR}/openssl/include)
include_directories(${LIBS_DIR}/rapidjson)
include_directories(${LIBS_DIR})
include_directories("C:/Program Files/PostgreSQL/12/include")
link_directories("C:\\Program Files\\PostgreSQL\\12\\lib")
include_directories("C:\\Program Files\\PostgreSQL\\9.6\\include")
add_executable(cars_web
    main.cpp

	stlplus/file_system.hpp
	stlplus/file_system.cpp
	stlplus/portability_fixes.hpp
	stlplus/portability_fixes.cpp
	stlplus/wildcard.hpp
	stlplus/wildcard.cpp

  managers/UserManager.h
  managers/UserManager.cpp
  managers/CarManager.h
  managers/CarManager.cpp
  managers/AutoPartManager.h
  managers/AutoPartManager.cpp
  managers/EventsManager.h
  managers/EventsManager.cpp
  managers/PQManager.h
  managers/PQManager.cpp
  managers/ESerie.h
  managers/ESerie.cpp
  managers/EMake.h
  managers/EMake.cpp
  managers/EModel.h
  managers/EModel.cpp
  managers/MessagesManager.h
  managers/MessagesManager.cpp

  routes/AuthRoute.h
  routes/AuthRoute.cpp
  routes/UsersRoute.h
  routes/UsersRoute.cpp
	routes/CarsRoute.h
  routes/CarsRoute.cpp
	routes/AutoPartsRoute.h
  routes/AutoPartsRoute.cpp
  routes/EventsRoute.h
  routes/EventsRoute.cpp
  routes/ImageUtils.h
  routes/ImageUtils.cpp
  routes/MessagesRoute.h
  routes/MessagesRoute.cpp

)

add_executable(cron_main
    cron_main.cpp
    managers/PQManager.h
    managers/PQManager.cpp
)

target_link_libraries(cars_web 
	#${LIBS_DIR}/pqxx/lib/x64/pqxx.lib
         webp
        webpdecoder
       # ${WebP_LIBRARIES}

     # libcrypto
       # libssl
	libpq
  cpr::cpr
	#pthread

	#	"C:\\Program Files\\PostgreSQL\\9.6\\lib\\libpq.lib"
	${LIBS_DIR}/openssl/lib/x64/libssl.lib
	${LIBS_DIR}/openssl/lib/x64/libcrypto.lib
)

target_link_libraries(cron_main
        #crypto
        #ssl
        libpq

       # pthread

        #	"C:\\Program Files\\PostgreSQL\\9.6\\lib\\libpq.lib"
#	${LIBS_DIR}/openssl/lib/x64/libssl.lib
#	${LIBS_DIR}/openssl/lib/x64/libcrypto.lib
)

