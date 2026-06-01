project(${CMAKE_PROJECT_NAME}-common)

add_library(${PROJECT_NAME} OBJECT)

find_package(Boost     REQUIRED)
find_package(spdlog    REQUIRED COMPONENTS header_only)
find_package(SQLiteCpp REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::headers spdlog::spdlog_header_only SQLiteCpp)

target_sources(${PROJECT_NAME}
    PRIVATE
        core/Exception.cpp
        network/Packet.cpp
        network/SocketListener.cpp
        services/Database.cpp
        utils/File.cpp
        utils/Misc.cpp
)
target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS
    FILES
        CompileTimeConfig.hpp
        Event.hpp
        OptRef.hpp
        Pimpl.hpp
        Typedefs.hpp
        core/Exception.hpp
        network/Packet.hpp
        network/PacketReader.hpp
        network/Serialization.hpp
        network/SocketListener.hpp
        services/Database.hpp
        utils/Enum.hpp
        utils/EnumMask.hpp
        utils/File.hpp
        utils/Misc.hpp
        utils/Traits.hpp
)
