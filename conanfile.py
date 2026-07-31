from conan             import ConanFile
from conan.tools.cmake import CMake, cmake_layout


class DefaultConanFile(ConanFile):
    type     = "unknown"
    settings = "os", "compiler", "build_type", "arch"

    requires = "boost/1.90.0", "openssl/1.1.1w", "spdlog/1.17.0", "sqlitecpp/3.3.3"
    default_options = {
        "boost/*:header_only": True,
        "spdlog/*:header_only": True,
        "spdlog/*:use_std_fmt": True,
    }

    generators = "CMakeConfigDeps", "CMakeToolchain"

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
