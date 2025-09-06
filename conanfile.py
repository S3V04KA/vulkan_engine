from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class hexRecipe(ConanFile):
    name = "hex"
    version = "0.0.1"
    package_type = "application"

    # Optional metadata
    license = "<Put the package license here>"
    author = "S3V04KA"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "Hex game"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    generators = "CMakeDeps"

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("glm/1.0.1")
        self.requires("vulkan-headers/1.4.313.0")

        if self.settings.os != "Android":
            self.requires("vulkan-loader/1.4.313.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    

    
