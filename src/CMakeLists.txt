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

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)

find_package(Boost COMPONENTS system thread REQUIRED)

if(Boost_FOUND)
	message(STATUS "Boost found")
	message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
	message(STATUS "Boost_VERSION: ${Boost_VERSION}")
	include_directories(${Boost_INCLUDE_DIRS})
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)


file(GLOB zxc_cpps "Game/*.cpp" "Utils/*.cpp" "*.cpp" "*.h")

add_executable(zxc ${zxc_cpps} Game/Event.cpp)

add_dependencies(zxc
		MagnumPlugins::AssimpImporter
		Magnum::AnyImageImporter
		Magnum::AnySceneImporter) # AnyScene is useless now btw but i will leave it here anyway

target_link_libraries(zxc PRIVATE
    ${Boost_LIBRARIES}
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Trade
    #Magnum::AnySceneImporter
    #Magnum::ObjImporter
    #Magnum::AnyImageImporter
    #Magnum::TgaImporter
    Magnum::Primitives
    #MagnumPlugins::AssimpImporter
        )

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT zxc)
