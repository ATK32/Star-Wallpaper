# StarWallpaper (C / Windows backend)

This project is an initial native Windows implementation of the wallpaper
prototype. It detects the running Windows version, offers a settings window,
lets the user select a local video, and applies it behind desktop icons using
the desktop shell's current WorkerW/Progman layout. Video decoding uses
VLC/libVLC with hardware decoding requested; the source file is played
directly without transcoding, so its encoded quality is retained. A separate
preview plays the selected file before it is applied.
Video sources are selected from the local computer.
The main window includes Apply and Clear controls. Closing it hides the
application in the system tray; the tray menu can reopen the main window,
open settings, show the About window, or exit. About includes a clickable
`TurkHackTeam.Org` link and the credit “Prepared & Developed by: @ATK”.
Left-clicking the tray icon opens the application window; the first tray-menu
item is labeled “Uygulamayı Aç”.
The About window includes the bold copyright notice “© 2026 Tüm hakları saklıdır.”
and application version “Uygulama Sürümü : v1.0”.
Settings include the Windows startup option, persisted in the Run registry key.
Video is always fitted to the desktop dimensions, cropping when needed. The process
sets DPI awareness before creating desktop windows so the wallpaper surface
uses physical display dimensions on scaled Windows desktops. The logo in
`assets\StarWallpaper.ico` is embedded into the executable with dedicated
16–256 px icon sizes and used by the main window and system tray. The About
window displays the provided mirrored logo above the centered clickable link
and developer credit on a white background.

## Important limits

- The current source implements **Windows only**. Linux desktop wallpapers
  need separate X11/Wayland integrations, and macOS needs a Cocoa integration.
  The current project does not claim support for those systems.
- The wallpaper backend requires an interactive Explorer desktop session;
  running as a service or in a session without the Windows desktop shell is
  unsupported and reported explicitly.
- The app enforces one running instance per interactive session; launching it
  again brings the existing settings window forward instead of adding another
  tray icon. Windows startup is triggered at user logon, not during boot. When
  a saved local video exists, VLC initialization starts in parallel with
  desktop-host discovery to reduce startup delay. Windows scheduling, Explorer
  readiness, VLC startup, and video decoding still prevent a guaranteed
  zero-delay first frame on every PC.
- Windows 11 24H2+ can host `SHELLDLL_DefView` directly under `Progman`.
  The backend checks that layout and places the video window below the icon
  view; older layouts use the sibling `WorkerW` host.
- The source and executable are configured for the Windows XP API/subsystem
  level. XP playback requires a matching VLC 2.2.x build; Windows 7 and newer
  should use a matching VLC 3.x build. However, the current Zig 0.17 builds
  import Universal CRT `api-ms-win-crt-*` DLLs, which Windows XP does not
  provide. Those binaries therefore do **not** run on XP. An XP-compatible
  MinGW/MSVC runtime is required for an XP build, and the result still needs
  testing on a real XP system.
- Fixed 20/40/65 MB playback ceilings cannot be guaranteed. Decoding requires
  VLC, codec buffers, and frame memory; actual usage depends on the video
  resolution, codec, VLC plugins, and graphics driver. The app
  selects local-file cache durations from Windows generation, physical and
  available RAM, and logical processor count: 700 ms on legacy or lower
  resource systems, 500 ms on mid-range systems, and 300 ms otherwise.
  These are cache durations, not RAM limits. A larger local-file buffer can
  reduce stutter caused by slow storage at the cost of a small increase in
  buffering memory. The app creates its wallpaper player only when Apply is
  pressed, avoiding an extra idle VLC player during preview.
- Hardware decoding is requested but cannot be guaranteed on every machine.
- VLC must be installed with the same 32/64-bit architecture as the app.
- Videos are fitted to the desktop aspect ratio and cropped when needed;
  scaling depends on the VLC build and graphics driver.

## Build on Windows

Install Visual Studio Build Tools with the C/C++ workload and CMake, then build
the 64-bit version:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
```

Windows PowerShell is used during the post-build step to embed the application
icon and About image resources into the executable.

For 32-bit XP targets, build with `-A Win32` and use a matching 32-bit
VLC 2.2.x installation. Windows 7 and newer should use the matching VLC 3.x
installation. Do not mix 32-bit and 64-bit binaries.

The executable is written to `build\Release\StarWallpaper.exe` with Visual
Studio generators. The app reads and applies the saved video path at startup.
The GNU/zig build explicitly links the Windows GUI subsystem so launching the
application does not create a console window.

## Windows installer

The Inno Setup script is `installer\StarWallpaper.iss`. Install Inno Setup 6.3
or newer, then run the VS Code task `Build StarWallpaper setup`; it builds both
app architectures and compiles the installer. The installer selects the x64
executable on 64-bit Windows and the x86 executable on 32-bit Windows. Its icon uses the
provided setup artwork, its welcome panel uses the mirrored StarWallpaper
logo, and its compatibility page displays the supplied Windows 7/8/10/11
logos.
The installer lets users select the application's language from 30 options:
Turkish, English, German, Arabic, Persian, French, Russian, Simplified
Chinese, Japanese, Portuguese, Spanish, Italian, Dutch, Polish, Ukrainian,
Greek, Hebrew, Korean, Hindi, Indonesian, Vietnamese, Thai, Swedish,
Norwegian, Danish, Finnish, Czech, Romanian, Hungarian, and Azerbaijani.
The selected language is stored per Windows user and applied to the app's
interface and messages. Turkish is the built-in fallback.
The installer exit code and application shortcuts are removed by uninstall.
The running app receives a dedicated shutdown request so its wallpaper player,
tray icon, and mutex are released before the program folder is deleted. The
app's `%APPDATA%\StarWallpaper` settings folder and startup registry value are
also removed; separately chosen video files are not deleted.

VLC is a separate prerequisite and is not bundled. Install VLC 3.x with the
same architecture as the installed StarWallpaper executable; newer major VLC
versions are not claimed as tested. Windows 7 SP1 is required and may also
need the Universal C Runtime update (KB2999226).

The installer labels 80–400 MB as an **unmeasured preliminary estimate** for
StarWallpaper plus VLC during 1080p playback, not a per-Windows benchmark or a
guarantee. Actual memory use depends on the video, codec, resolution, hardware
decoding, and graphics driver; 4K video can require more.
