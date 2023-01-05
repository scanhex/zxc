# Find Assimp and Magnum
find_package(Assimp)
find_package(
        Magnum REQUIRED
        GL
        Sdl2Application
        SceneGraph
        Trade
        AnySceneImporter
        TgaImporter
        Primitives
        MeshTools
        Text
)
find_package(MagnumPlugins REQUIRED AssimpImporter StbTrueTypeFont)
find_package(MagnumExtras REQUIRED)

# Find Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system thread regex REQUIRED)

if (Boost_FOUND)
    message(STATUS "Boost found")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

# Set Corrade flags
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Make zxc and zxc-server targets
file(GLOB shared "shared/**/*.h" "shared/**/*.cpp")
file(GLOB zxc_sources "zxc/*.cpp" "zxc/*.h" "zxc/**/*.cpp" "zxc/**/*.h" ${shared})
file(GLOB server_sources "zxc-server/*.cpp" "zxc-server/*.h" ${shared})

add_executable(zxc ${zxc_sources})
add_executable(zxc-server ${server_sources})

target_include_directories(zxc PRIVATE zxc shared)
target_include_directories(zxc-server PRIVATE zxc-server shared)

# Zxc dependencies
add_dependencies(
        zxc
        Magnum::AnyImageImporter
        Magnum::TgaImporter
        MagnumPlugins::AssimpImporter
        MagnumPlugins::StbTrueTypeFont
)

# Platform-specific libraries
if (MSVC)
    target_link_libraries(zxc PRIVATE Magnum::AnyImageImporter MagnumPlugins::AssimpImporter MagnumPlugins::StbTrueTypeFont)
endif ()

IF (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L/usr/local/opt/icu4c/lib")
ENDIF ()

# Link libraries
target_link_libraries(
        zxc PRIVATE
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
        MagnumExtras::Ui
)

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT zxc)
