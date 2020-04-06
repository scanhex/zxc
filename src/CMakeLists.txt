find_package(Magnum REQUIRED
        GL
        Sdl2Application
        SceneGraph
        Trade
        AnySceneImporter
        ObjImporter
		TgaImporter
        Primitives
        MeshTools)
find_package(Assimp)
find_package(MagnumPlugins REQUIRED AssimpImporter)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_subdirectory(Game)

file(GLOB_RECURSE zxc_cpps "*.cpp" "*.h")
add_executable(zxc ${zxc_cpps})
target_link_libraries(zxc PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Trade
    Magnum::AnySceneImporter
    Magnum::ObjImporter
	Magnum::TgaImporter
	Magnum::Primitives
    MagnumPlugins::AssimpImporter
        )

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT zxc)
