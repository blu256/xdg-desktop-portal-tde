# xdg-desktop-portal-tde

## What is this?

This is an implementation of the [FreeDesktop Portals API](https://flatpak.github.io/xdg-desktop-portal).

THIS IS AN EARLY PROTOTYPE. USE WITH CARE.

This implementation exposes TDE APIs to applications which use the Portals API. As a result, these applications
can transparently use native APIs and dialogs.

Read more about the portals API on its official [website](https://flatpak.github.io/xdg-desktop-portal/docs/introduction.html).

## How do I use this?
Before using this, make sure you have the `xdg-desktop-portal` package installed and running on your system.

To make Firefox use the Portals API, go to `about:config`, search for options that start with "widget.use-xdg-desktop-portal" and,
if they are set to 2, set them to 1.

## State of implementation

Currently implemented are the following interfaces:
* FileChooser - request a file dialog

The following implementations are planned (listed in no particular order):
* Screenshot - request a screenshot
* App Chooser - show a "Open with..." dialog for a file
* Clipboard - request clipboard access (*)
* Email - request to send an e-mail via the system e-mail client
* OpenURI - request to open a URL
* Secret - integration with TDEWallet
* Account - request user information (username, full name, user icon)
* Access - show a grant access dialog
* Wallpaper - request to change the user's wallpaper
* Print - request a file to be printed

(*) Needs Session interface implementation

Additionaly, it must be noted that the implementation does not currently support the Documents portal API (and therefore sandboxed applications).
