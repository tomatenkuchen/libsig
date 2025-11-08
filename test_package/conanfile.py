import os
from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMakeToolchain, CMake, CMakeDeps, cmake_layout
from conan.tools.files import copy
from pathlib import Path


class libsigTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires(self.tested_reference_str)
        self.tool_requires("cmake/4.1.2")
        self.tool_requires("ninja/1.13.1")
        self.tool_requires("cppcheck/2.18.3")
        self.test_requires("catch2/3.11.0")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator="Ninja")
        # make cmake emit compile commands
        tc.cache_variables["CMAKE_EXPORT_COMPILE_COMMANDS"] = "ON"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()

        # copy compile commands file to a location that clangd can find it
        build_path = Path(self.build_folder)
        copy(self, "compile_commands.json", build_path, build_path / "..")

        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            cmd = Path(self.cpp.build.bindir) / "tests"
            self.run(cmd, env="conanrun")
