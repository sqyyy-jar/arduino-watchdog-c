{pkgs ? import <nixpkgs> {}}:

pkgs.mkShell {
    buildInputs = with pkgs; [
        arduino-cli
        avrdude
        pkgsCross.avr.buildPackages.gcc
    ];
}
