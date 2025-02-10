from conan import ConanFile, tools
from conan.tools.cmake import cmake_layout, CMakeDeps, CMakeToolchain, CMake


class nikaRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    @property
    def _run_tests(self):
        return tools.get_env("CONAN_RUN_TESTS", False)
    
    def requirements(self):
        self.requires("sc-machine/0.10.0")
        self.requires("scl-machine/0.3.0")
        self.requires("nlohmann_json/3.11.3")
        self.requires("libcurl/8.11.1")

    def build_requirements(self):
        self.test_requires("gtest/1.14.0")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()  # equivalent to self.run("cmake . <other args>")
        cmake.build()

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.user_presets_path = "ConanPresets.json"
        tc.generate()
