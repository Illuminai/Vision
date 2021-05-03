#include "windowing/WindowManager.h"

#include "example/ImGuiExampleWindow.h"
#include "example/RaytracingExampleWindow.h"
#include <shaderc/shaderc.hpp>
#include <cmrc/cmrc.hpp>

CMRC_DECLARE(shaders);

int main() {
    auto manager = windowing::WindowManager::getInstance();

    //manager->addWindow(std::make_shared<RaytracingExampleWindow>());
    manager->addWindow(std::make_shared<ImGuiExampleWindow>());

    manager->startManager();

    /*auto fs = cmrc::shaders::get_filesystem();
    auto fragFile = fs.open("shaders/rasterization/basic.frag");

    std::string fragContent{fragFile.begin(), fragFile.end()};



    shaderc::Compiler compiler;

    // Shaderc preprocessing
    {
        shaderc::CompileOptions options;

        options.AddMacroDefinition("MY_DEFINE", "1");

        shaderc::PreprocessedSourceCompilationResult result =
                compiler.PreprocessGlsl(fragContent, shaderc_glsl_vertex_shader, "t", options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            std::cerr << result.GetErrorMessage();
        }

        std::string text{result.cbegin(), result.cend()};

        std::cout << text << std::endl;
    }

    // Shaderc compile to spirv assembly
    {
        shaderc::CompileOptions options;

        options.AddMacroDefinition("MY_DEFINE", "1");

        // Optimization level
        options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(
                fragContent, shaderc_glsl_vertex_shader, "t", options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            std::cerr << result.GetErrorMessage();
        }

        std::string text{result.cbegin(), result.cend()};

        std::cout << text << std::endl;
    }

    // Shaderc compile to spirv
    {
        shaderc::CompileOptions options;

        options.AddMacroDefinition("MY_DEFINE", "1");
        options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
                fragContent, shaderc_glsl_vertex_shader, "t", options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            std::cerr << result.GetErrorMessage();
        }

        std::string text{result.cbegin(), result.cend()};

        std::cout << text << std::endl;
    }*/

    return 0;
}