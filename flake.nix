# flake.nix
{
  description = "A development environment for the zxc project";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; # Or a specific release like "nixos-23.11"
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        boost = pkgs.boost187;

        # List of dependencies based on the README
        # We'll use the versions available in nixpkgs.
        # If specific old versions are strictly required, more work would be needed.
        projectDeps = [
          # Boost components (or just pkgs.boost which includes common ones)
          boost
          # Graphics and Windowing
          pkgs.libGL 
          pkgs.SDL2
          pkgs.clang_19
          # Dear ImGui (Magnum also has ImGui integration, but zxc lists it separately)
          pkgs.imgui
          pkgs.xorg.libX11 
          # Asset Importer
          pkgs.assimp
          # stb_truetype.h (if not vendored by the project)
          # pkgs.stb # Uncomment if stb_truetype.h is not found by the build
        ];

        # Build tools
        buildTools = [
          pkgs.cmake
          pkgs.pkg-config
          pkgs.git # Often useful for CMake projects, e.g., to fetch submodules or get version info
                   # Although for flakes, submodules of `self` are handled by Nix itself.
        ];
      in
      {
        # Development shell
        # Access with `nix develop`
        devShells.default = pkgs.mkShell {
          packages = buildTools ++ projectDeps;

          # Environment variables that might be useful or required by the build system
          # BOOST_ROOT is often helpful for CMake's FindBoost module.
          # Nix normally sets up paths so these explicit settings are often not strictly needed,
          # but they don't hurt if CMake looks for them.
           BOOST_ROOT = "${boost.out}";
           BOOST_INCLUDEDIR = "${boost.out}/include";
           BOOST_LIBRARYDIR = "${boost.out}/lib"; # Check actual path with `ls ${pkgs.boost.lib}`

          shellHook = ''
            export CC=${pkgs.clang_16}/bin/clang
            export CXX=${pkgs.clang_16}/bin/clang++
            echo ">>> Welcome to the zxc development environment!"
            echo ">>> Submodules should be initialized if this flake is in the project root."
            echo ">>> Otherwise, run: git submodule update --init --recursive"
            echo ""
            echo ">>> To build zxc (e.g., Release):"
            echo "  mkdir -p build && cd build"
            echo "  cmake -DCMAKE_BUILD_TYPE=Release .."
            echo "  make"
            echo "  ./bin/zxc  # (Assuming output is build/bin/zxc)"
            echo ""
            echo ">>> To run a local server (after building):"
            echo "  ./bin/zxc-server"
            echo ">>> And ensure 'resources/config.ini' has:"
            echo "  SERVER_IP = 127.0.0.1"
            echo ""
            echo ">>> Using Boost from nixpkgs: ${boost.out}"
            echo ">>> Using SDL2 from nixpkgs: ${pkgs.SDL2.version}"
          '';
        };

        # You can also define Nix packages for zxc and zxc-server
        packages.zxc-app = pkgs.stdenv.mkDerivation {
          pname = "zxc";
          version = "0.1.0-dev"; # Replace with actual version or git describe
          src = self; # Assumes this flake.nix is in the root of zxc project

          nativeBuildInputs = buildTools;
          buildInputs = projectDeps;

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
            # Add other CMake flags if necessary
            # e.g. if CMake has trouble finding Boost:
            # "-DBOOST_ROOT=${pkgs.boost.dev}" # .dev for headers, .lib for libs
            # "-DBOOST_INCLUDEDIR=${pkgs.boost.dev}/include"
            # "-DBOOST_LIBRARYDIR=${pkgs.boost.lib}/lib"
          ];

          # If CMake doesn't have a proper install target, you might need to manually copy files.
          # Assuming standard build output paths from the README (e.g., ./Release/bin/zxc)
          # This installPhase assumes building in-source or that cmake output dir is predictable.
          # A common practice is to build in a subdirectory like 'build'.
          # Nix's stdenv.mkDerivation typically creates a 'build' dir automatically.
          # Let's adjust for the CMake commands given:
          # cmake -DCMAKE_BUILD_TYPE=Release .
          # make
          # ./Release/bin/zxc
          # This implies the build is NOT in a separate 'build' directory from src root by default for 'Release'.
          # However, stdenv.mkDerivation unpacks src and cd's into it, so `.` refers to src root.
          # If CMake creates a "Release" directory at the top level:

          # It's better if CMake has an install phase. If not:
          installPhase = ''
            runHook preInstall

            # The README implies binaries are put into a 'Release' subdir relative to build root
            # For stdenv.mkDerivation, the build happens in a temporary directory where src is copied.
            # So, if `make` produces `./Release/bin/zxc`, we copy from there.
            mkdir -p $out/bin
            cp Release/bin/zxc $out/bin/zxc
            cp Release/bin/zxc-server $out/bin/zxc-server

            # If there are resources to install (e.g., from a 'resources' directory)
            if [ -d resources ]; then
              mkdir -p $out/share/zxc
              cp -r resources $out/share/zxc/
            fi

            runHook postInstall
          '';

          # If the project needs resources at runtime relative to the executable,
          # you might need to patch the executable or provide a wrapper script.
        };
        packages.default = self.packages.${system}.zxc-app;

      });
}
