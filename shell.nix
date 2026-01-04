{ pkgs ? import <nixpkgs> {} }:

let
  python = pkgs.python39; # chosen as stable, change to python310/311 if you prefer
  pipx = python.pkgs.pipx;
in
pkgs.mkShell {
  name = "cab-esp-dev-shell";

  buildInputs = with pkgs; [
    openjdk17
    gradle
    git
    wget
    unzip
    zip
    cmake
    ninja
    # Python & helpers
    python
    pipx
    # ESP helpers
    esptool
    picocom
    gcc-arm-none-eabi
  ];

  # Useful environment defaults for Android SDK and Java
  shellHook = ''
    # Ensure JAVA_HOME is set for Gradle/AGP
    export JAVA_HOME=${pkgs.openjdk17}

    # Default Android SDK location used by many tools (you can customize)
    export ANDROID_SDK_ROOT="$HOME/Android"
    export ANDROID_HOME="$ANDROID_SDK_ROOT"

    # If you already installed Android command-line tools, prefer them on PATH
    if [ -d "$ANDROID_SDK_ROOT/cmdline-tools/latest/bin" ]; then
      export PATH="$ANDROID_SDK_ROOT/cmdline-tools/latest/bin:$ANDROID_SDK_ROOT/platform-tools:$PATH"
    else
      # hint to install cmdline-tools with sdkmanager if needed
      :
    fi

    # Make pipx-managed commands available in the shell (if pipx is installed)
    if command -v pipx >/dev/null 2>&1; then
      PIPX_BIN_DIR="$HOME/.local/bin"
      export PATH="$PIPX_BIN_DIR:$PATH"
      # Ensure platformio is available via pipx (install if missing)
      if ! command -v pio >/dev/null 2>&1; then
        echo "Installing PlatformIO (pipx) into $PIPX_BIN_DIR (one-time)"
        pipx install platformio || true
      fi
    fi

    # Helpful reminders
    echo "[dev-shell] JAVA_HOME=$JAVA_HOME" >&2
    echo "[dev-shell] ANDROID_SDK_ROOT=$ANDROID_SDK_ROOT (place Android cmdline-tools under $ANDROID_SDK_ROOT/cmdline-tools/latest)" >&2
    echo "[dev-shell] PlatformIO CLI: $(command -v pio || echo 'not installed yet; will attempt pipx install')" >&2
  '';

  # Optional: quieter nix output
  # shellHook = "export NIXPKGS_ALLOW_UNFREE=1";
}
