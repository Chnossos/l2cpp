project(login_server)

add_executable(${PROJECT_NAME})

find_package(OpenSSL REQUIRED COMPONENTS Crypto)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}-common OpenSSL::Crypto)

set_property(SOURCE main.cpp APPEND PROPERTY
    COMPILE_DEFINITIONS
        L2CPP_SUBPROJECT_NAME="${PROJECT_NAME}"
        L2CPP_VERSION="${CMAKE_PROJECT_VERSION}"
        L2CPP_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
        L2CPP_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
        L2CPP_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
        L2CPP_VERSION_TWEAK=${CMAKE_PROJECT_VERSION_TWEAK}
)

target_sources(${PROJECT_NAME}
    PRIVATE
        $<TARGET_OBJECTS:${CMAKE_PROJECT_NAME}-common>
    PRIVATE
        main.cpp
        Constants.hpp
        Orm.cpp
        Orm.hpp
        network/Connection.cpp
        network/Connection.hpp
        network/OpCodes.hpp
        crypto/Blowfish.cpp
        crypto/Blowfish.hpp
        crypto/Checksum.cpp
        crypto/Checksum.hpp
        crypto/Rsa.cpp
        crypto/Rsa.hpp
)
