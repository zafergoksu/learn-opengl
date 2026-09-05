from conan import ConanFile
from conan.tools.cmake import cmake_layout


class LearnOpenglConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def configure(self):
        self.options["glad"].spec = "gl"
        self.options["glad"].gl_profile = "core"
        self.options["glad"].gl_version = "4.1"

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("glad/0.1.36")

    def layout(self):
        cmake_layout(self)
