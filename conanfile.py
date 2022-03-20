import os
from conans import ConanFile, CMake, tools


class BongoMountainConan(ConanFile):
    name = "BongoMountain"
    version = "0.0.0"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of BongoMountain here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    requires = "glfw/3.3.6", "glad/0.1.34", "glm/0.9.9.8", "imgui/1.86", "spdlog/1.9.2", "benchmark/1.6.0", "easy_profiler/2.1.0", "nlohmann_json/3.10.5", "stb/cci.20210910"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True, "glad:gl_version": 4.5, "glad:shared": True, "imgui:shared": True,
                       "glfw:shared": True, "easy_profiler:shared": True, "spdlog:shared": True}
    generators = "cmake_find_package_multi", "cmake_paths"
    no_copy_source = True

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=self.source_folder)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = []

    def imports(self):
        if self.build_folder is None:
            bindings_dir = "bindings"
        else:
            bindings_dir = os.path.join(self.build_folder, "bindings")
        self.copy("imgui_impl_glfw.cpp", src="./res/bindings", dst=bindings_dir)
        self.copy("imgui_impl_glfw.h", src="./res/bindings", dst=bindings_dir)
        self.copy("imgui_impl_opengl3.cpp", src="./res/bindings", dst=bindings_dir)
        self.copy("imgui_impl_opengl3.h", src="./res/bindings", dst=bindings_dir)
        self.copy("imgui_impl_opengl3_loader.h", src="./res/bindings", dst=bindings_dir)

        self.copy("*.dll", src="./bin", dst="./bin")
        self.copy("*.exe", src="./bin", dst="./bin")
