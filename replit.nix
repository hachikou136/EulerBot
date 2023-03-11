{ pkgs }: {
	deps = [
		pkgs.mlocate
  pkgs.vimHugeX
  pkgs.sudo
  pkgs.nodejs-18_x
    pkgs.nodePackages.typescript-language-server
    pkgs.yarn
    pkgs.replitPackages.jest
    pkgs.glib
    pkgs.chromium
    # pkgs.chromiumdriver
    pkgs.dbus
    pkgs.nss
    pkgs.fontconfig
    # pkgs.chromium-browser
	];
}