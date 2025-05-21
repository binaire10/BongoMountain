import os
from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, CMake, cmake_layout
from conan.tools.files import copy


class BongoMountainConan(ConanFile):
    name = "BongoMountain"
    version = "0.0.0"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True, "glad/*:gl_version": 4.5, "glad/*:shared": True,
                       "imgui/*:shared": True,
                       "glfw/*:shared": True, "spdlog/*:shared": True}
    build_type = "Release"
    no_copy_source = True

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("glad/0.1.36")
        self.requires("glm/1.0.1")
        self.requires("imgui/1.91.8")
        self.requires("spdlog/1.15.1")
        self.requires("benchmark/1.9.1")
        self.requires("nlohmann_json/3.12.0")
        self.requires("stb/cci.20240531")
    def config_options(self):
        if self.settings.get_safe("os") == "Windows":
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        if self.build_folder is None:
            bindings_dir = "bindings"
        else:
            bindings_dir = os.path.join(self.build_folder, "bindings")
        imgui_folder = self.dependencies["imgui"].package_folder
        copy(self, "imgui_impl_glfw.cpp", src=os.path.join(imgui_folder, "res/bindings"), dst=bindings_dir)
        copy(self, "imgui_impl_glfw.h", src=os.path.join(imgui_folder, "res/bindings"), dst=bindings_dir)
        copy(self, "imgui_impl_opengl3.cpp", src=os.path.join(imgui_folder, "res/bindings"), dst=bindings_dir)
        copy(self, "imgui_impl_opengl3.h", src=os.path.join(imgui_folder, "res/bindings"), dst=bindings_dir)
        copy(self, "imgui_impl_opengl3_loader.h", src=os.path.join(imgui_folder, "res/bindings"), dst=bindings_dir)

        copy(self, "*.so", src="./bin", dst=os.path.join(bindings_dir,"bin"))
        copy(self, "*.dll", src="./bin", dst=os.path.join(bindings_dir,"bin"))
        copy(self, "*.exe", src="./bin", dst=os.path.join(bindings_dir,"bin"))


        for r, d in self.dependencies.items():
            if d.package_folder is None:
                continue
            # look for .dlls and .exes in the bin folder
            copy(self, "*.so",  src=os.path.join(d.package_folder, "bin"), dst=os.path.join(self.build_folder,"bin"))
            copy(self, "*.dll", src=os.path.join(d.package_folder, "bin"), dst=os.path.join(self.build_folder,"bin"))
            copy(self, "*.exe", src=os.path.join(d.package_folder, "bin"), dst=os.path.join(self.build_folder,"bin"))

        cmake = CMakeDeps(self)
        cmake.generate()

        tc = CMakeToolchain(self)
        tc.variables["IMGUI_SRC_DIR"] = bindings_dir.replace("\\", "/")
        tc.generate()


    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = []

