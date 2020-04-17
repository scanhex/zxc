find_package(Magnum REQUIRED
        GL
        Sdl2Application
        SceneGraph
        Trade
        AnySceneImporter
        ObjImporter
	TgaImporter
        Primitives
        MeshTools
        Text
        )
find_package(Assimp)
find_package(MagnumPlugins REQUIRED AssimpImporter StbTrueTypeFont)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)

find_package(Boost COMPONENTS system thread regex REQUIRED)

if(Boost_FOUND)
	message(STATUS "Boost found")
	message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
	message(STATUS "Boost_VERSION: ${Boost_VERSION}")
	include_directories(${Boost_INCLUDE_DIRS})
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)


file(GLOB zxc_cpps "Game/*.cpp" "Utils/*.cpp" "Client/*.cpp" "Graphics/*.cpp" "*.cpp" "*.h" "Graphics/*.h")

file(GLOB server_cpps "Server/main.cpp" "Server/Server.cpp" "Server/Server.h" "Utils/*.cpp" "Utils/*.h" "Game/*.cpp" "Game/*.h")

add_executable(Server ${server_cpps})

add_executable(zxc ${zxc_cpps})

add_dependencies(zxc
    Magnum::AnyImageImporter
    MagnumPlugins::AssimpImporter
    MagnumPlugins::StbTrueTypeFont
        )

if (MSVC)
	target_link_libraries(zxc PRIVATE Magnum::AnyImageImporter MagnumPlugins::AssimpImporter MagnumPlugins::StbTrueTypeFont)
endif()


if(Boost_FOUND)
    message(STATUS "Linking boost libraries")
    target_link_libraries(Server ${Boost_LIBRARIES})

endif()

target_link_libraries(zxc PRIVATE
    ${Boost_LIBRARIES}
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Trade
    Magnum::Primitives
    Magnum::Text
        )

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT zxc)
