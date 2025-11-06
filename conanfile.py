import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, CMakeDeps, cmake_layout
from conan.tools.files import copy
from conan.tools.build import check_min_cppstd

class Libsig(ConanFile):
    name = "libsig"
    version = "0.0.2"
    author = "tomatenkuchen"
    url = "https//:localhost"
    license = "private"
    description = "signal processing library"
    topics = ("filter", "power analyzer")
    exports_sources = "include/*", "test/*"
    no_copy_source = True
    generators = "CMakeDeps", "CMakeToolchain"
    settings = "os", "build_type", "arch", "compiler"

    def requirements(self):
        self.requires("mp-units/0.8.0")
        self.test_requires("catch2/3.3.2")

    def validate(self):
        check_min_cppstd(self, 20)

    def layout(self):
        cmake_layout(self)

    def build(self):
        if not self.conf.get("tools.build:skip_test", default=False):
            cmake = CMake(self)
            cmake.configure(build_script_folder="test")
            cmake.build()
            self.run(os.path.join(self.cpp.build.bindir, "tests"))

    def package(self):
        copy(self, "*.hpp", self.source_folder, self.package_folder)

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []

    def package_id(self):
        self.info.clear()
