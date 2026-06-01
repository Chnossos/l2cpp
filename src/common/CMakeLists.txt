project(${CMAKE_PROJECT_NAME}-common)

add_library(${PROJECT_NAME} OBJECT)

find_package(Boost     REQUIRED)
find_package(spdlog    REQUIRED COMPONENTS header_only)
find_package(SQLiteCpp REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::headers spdlog::spdlog_header_only SQLiteCpp)

target_sources(${PROJECT_NAME}
    PRIVATE
               l2cpp/core/Exception.cpp
               l2cpp/network/Packet.cpp
               l2cpp/network/SocketListener.cpp
               l2cpp/services/Database.cpp
               l2cpp/utils/File.cpp
               l2cpp/utils/Misc.cpp
)
target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS
    FILES
               l2cpp/CompileTimeConfig.hpp
               l2cpp/Event.hpp
               l2cpp/OptRef.hpp
               l2cpp/Pimpl.hpp
               l2cpp/Typedefs.hpp
               l2cpp/core/Exception.hpp
               l2cpp/network/Packet.hpp
               l2cpp/network/PacketReader.hpp
               l2cpp/network/Serialization.hpp
               l2cpp/network/SocketListener.hpp
               l2cpp/services/Database.hpp
               l2cpp/utils/Enum.hpp
               l2cpp/utils/EnumMask.hpp
               l2cpp/utils/File.hpp
               l2cpp/utils/Misc.hpp
               l2cpp/utils/Traits.hpp
)
