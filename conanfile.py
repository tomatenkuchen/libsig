from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMake, CMakeDeps, cmake_layout
from conan.tools.files import copy
from conan.tools.scm import Git
from pathlib import Path
import os


class libsigRecipe(ConanFile):
    name = "libsig"
    version = "0.0.0"
    package_type = "library"

    # Optional metadata
    license = "GPL-2.0"
    author = "tomatenkuchen"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "signal processing library"
    topics = ("filter", "controller")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "docs/*"

    hash = None

    def set_version(self):
        git = Git(self, self.recipe_folder)
        self.version = git.run("describe --tags").split('-')[0]
        self.hash = git.run("rev-parse HEAD")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def requirements(self):
        self.tool_requires("cmake/4.1.2")
        self.tool_requires("ninja/1.13.1")
        self.tool_requires("cppcheck/2.18.3")
        self.requires("mp-units/2.4.0")

    def layout(self):
        cmake_layout(self)
    
    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator="Ninja")

        # add project metadata to cmake
        tc.cache_variables["CONAN_PROJECT_NAME"] = str(self.name)
        tc.cache_variables["CONAN_PROJECT_VERSION"] = str(self.version)
        tc.cache_variables["CONAN_PROJECT_DESCRIPTION"] = str(self.description)
        tc.cache_variables["CONAN_PROJECT_GIT_HASH"] = str(self.hash)

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

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.requires = ["mp-units::mp-units"]
