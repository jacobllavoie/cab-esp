{ pkgs ? import <nixpkgs> {} }:

let
  python = pkgs.python39;
  pipx = python.pkgs.pipx;
in
pkgs.mkShell {
  name = "cab-esp-esp-shell";

  buildInputs = with pkgs; [
    python
    pipx
    git
    cmake
    ninja
    esptool
    picocom
    gcc-arm-none-eabi
    # If your board uses a RISC-V toolchain (esp32-c6), PlatformIO usually installs it,
    # but you might want riscv64-unknown-elf-toolchain if you do system-level builds.
    # riscv64-unknown-elf-gcc
  ];

  shellHook = ''
    # Make pipx-managed commands available
    PIPX_BIN_DIR="$HOME/.local/bin"
    export PATH="$PIPX_BIN_DIR:$PATH"

    # Install PlatformIO via pipx if missing
    if ! command -v pio >/dev/null 2>&1; then
      echo "Installing PlatformIO (pipx) into $PIPX_BIN_DIR (one-time)"
      python -m pip install --user pipx || true
      ~/.local/bin/pipx ensurepath || true
      pipx install platformio || true
    fi

    echo "[esp-shell] pio: $(command -v pio)" >&2
    echo "[esp-shell] esptool: $(command -v esptool || echo 'esptool not found')" >&2
  '';
}
