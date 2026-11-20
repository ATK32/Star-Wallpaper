#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#ifdef WINVER
#undef WINVER
#endif
#define _WIN32_WINNT 0x0501
#define WINVER 0x0501
#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define APP_NAME L"StarWallpaper"
#define WINDOW_CLASS L"StarWallpaperMainWindow"
#define SETTINGS_CLASS L"StarWallpaperSettingsWindow"
#define ABOUT_CLASS L"StarWallpaperAboutWindow"
#define WALLPAPER_CLASS L"StarWallpaperSurface"
#define SETTINGS_SECTION L"Settings"
#define WM_APPLY_SAVED_VIDEO (WM_APP + 1)
#define WM_TRAY_CALLBACK (WM_APP + 2)
#define WM_SHOW_MAIN_WINDOW (WM_APP + 3)
#define WM_VLC_PREWARM_COMPLETE (WM_APP + 4)
#define WM_UNINSTALL_SHUTDOWN (WM_APP + 5)
#define ID_VIDEO_PATH 1001
#define ID_BROWSE 1002
#define ID_APPLY 1003
#define ID_CLEAR 1004
#define ID_SETTINGS 1005
#define ID_STATUS 1006
#define ID_TRAY_OPEN 1007
#define ID_TRAY_EXIT 1008
#define ID_ABOUT 1009
#define IDI_APP_LOGO 101
#define ID_SETTINGS_AUTOSTART 1101
#define ID_SETTINGS_SAVE 1103
#define ID_SETTINGS_CANCEL 1104
#define ID_ABOUT_LINK 1201
#define IDB_ABOUT_LOGO 102
#define ID_ABOUT_CREDIT 1202
#define ID_ABOUT_COPYRIGHT 1203
#define ID_ABOUT_VERSION 1204

typedef enum {
    TXT_MAIN_TITLE,
    TXT_SETTINGS_TITLE,
    TXT_ABOUT_TITLE,
    TXT_OS_LABEL,
    TXT_PREVIEW_HINT,
    TXT_VIDEO_LABEL,
    TXT_BROWSE,
    TXT_APPLY,
    TXT_CLEAR,
    TXT_SETTINGS,
    TXT_ABOUT,
    TXT_INITIAL_STATUS,
    TXT_CACHE_STATUS,
    TXT_TRAY_FAILED,
    TXT_TRAY_TIP,
    TXT_TRAY_OPEN,
    TXT_TRAY_VIDEO,
    TXT_TRAY_ABOUT,
    TXT_TRAY_EXIT,
    TXT_PREVIEW_START_FAILED,
    TXT_VIDEO_OPEN_FAILED,
    TXT_PREVIEW_CREATE_FAILED,
    TXT_PREVIEW_PLAY_FAILED,
    TXT_PREVIEW_READY,
    TXT_CLEAR_FAILED,
    TXT_VIDEO_CLEARED,
    TXT_SAVE_FAILED,
    TXT_VIDEO_APPLIED,
    TXT_APPLY_WAIT,
    TXT_CHOOSE_VIDEO,
    TXT_FILE_MISSING,
    TXT_VLC_START_FAILED,
    TXT_PATH_CONVERT_FAILED,
    TXT_PLAYER_CREATE_FAILED,
    TXT_PLAYBACK_FAILED,
    TXT_VIDEO_FILTER,
    TXT_ALL_FILES,
    TXT_VIDEO_DIALOG_TITLE,
    TXT_SETTINGS_CREATE_FAILED,
    TXT_AUTOSTART,
    TXT_SAVE,
    TXT_CANCEL,
    TXT_SETTINGS_SAVE_FAILED,
    TXT_ABOUT_CREATE_FAILED,
    TXT_ABOUT_LOGO_FAILED,
    TXT_VERSION,
    TXT_WEBSITE_FAILED,
    TXT_VLC_PREWARM_READY,
    TXT_VLC_PREWARM_FAILED,
    TXT_VLC_INIT_FAILED,
    TXT_SAVED_VIDEO_MISSING,
    TXT_SETTINGS_PATH_FAILED,
    TXT_DESKTOP_HOST_FAILED,
    TXT_WALLPAPER_WINDOW_FAILED,
    TXT_WINDOW_CREATE_FAILED,
    TXT_LANGUAGE_LOAD_FAILED,
    TXT_COUNT
} TextId;

static const char *const g_text_keys[TXT_COUNT] = {
    "main_title", "settings_title", "about_title", "os_label",
    "preview_hint", "video_label", "browse", "apply", "clear",
    "settings", "about", "initial_status", "cache_status", "tray_failed",
    "tray_tip", "tray_open", "tray_video", "tray_about", "tray_exit",
    "preview_start_failed", "video_open_failed", "preview_create_failed",
    "preview_play_failed", "preview_ready", "clear_failed", "video_cleared",
    "save_failed", "video_applied", "apply_wait", "choose_video",
    "file_missing", "vlc_start_failed", "path_convert_failed",
    "player_create_failed", "playback_failed", "video_filter",
    "all_files", "video_dialog_title", "settings_create_failed",
    "autostart", "save", "cancel", "settings_save_failed",
    "about_create_failed", "about_logo_failed", "version",
    "website_failed", "vlc_prewarm_ready",
    "vlc_prewarm_failed", "vlc_init_failed", "saved_video_missing",
    "settings_path_failed", "desktop_host_failed",
    "wallpaper_window_failed", "window_create_failed",
    "language_load_failed"
};

static const wchar_t *const g_default_texts[TXT_COUNT] = {
    L"Star Wallpaper - Ayarlar",
    L"Star Wallpaper - Ayarlar",
    L"Star Wallpaper - Hakkında",
    L"İşletim sistemi:",
    L"Önizleme Uygula'ya kadar masaüstünü değiştirmez.",
    L"Arka plan videosu:",
    L"Gözat...",
    L"Uygula",
    L"Sil",
    L"Ayarlar...",
    L"Hakkında",
    L"Bilgisayarınızdan bir video dosyası seçin.",
    L"Otomatik yerel video profili seçildi (%u ms tampon).",
    L"Sistem tepsisi başlatılamadı; pencereyi kapatırsanız program çıkar.",
    L"Star Wallpaper",
    L"Uygulamayı Aç",
    L"Video ayarları...",
    L"Hakkında",
    L"Çıkış",
    L"Önizleme başlatılamadı; Windows sürümünüze uygun VLC kurulumunu kontrol edin.",
    L"VLC seçilen dosyayı açamadı.",
    L"Video önizleme oynatıcısı oluşturulamadı.",
    L"VLC video önizlemesini başlatamadı.",
    L"Önizleme hazır. Masaüstüne uygulamak için Uygula'ya basın.",
    L"Video ayarı diske silinemedi.",
    L"Arka plan videosu kaldırıldı.",
    L"Video ayarı diske kaydedilemedi.",
    L"Video uygulandı; bellek kullanımı codec ve çözünürlüğe bağlı.",
    L"Video oynatıcı hazırlanıyor; arka plan hazır olduğunda uygulanacak.",
    L"Önce bilgisayarınızdan bir video dosyası seçin.",
    L"Seçilen video dosyası bulunamadı.",
    L"VLC oynatıcı kitaplığı bulunamadı veya başlatılamadı. VLC 3.x'in kurulu olduğunu ve mimarisinin uygulamayla aynı (32/64 bit) olduğunu kontrol edin.",
    L"Video yolu dönüştürülemedi veya VLC dosyayı açamadı.",
    L"VLC video oynatıcısı oluşturulamadı.",
    L"VLC videoyu başlatamadı. Codec, dosya veya donanım hızlandırma uyumluluğunu kontrol edin.",
    L"Video dosyaları",
    L"Tüm dosyalar",
    L"Arka plan videosu seçin",
    L"Ayarlar penceresi oluşturulamadı.",
    L"Windows açıldığında otomatik başlat",
    L"Kaydet",
    L"İptal",
    L"Windows başlangıç ayarı kaydedilemedi.",
    L"Hakkında penceresi oluşturulamadı.",
    L"Hakkında logosu yüklenemedi.",
    L"Uygulama Sürümü : v1.0",
    L"Web sitesi tarayıcıda açılamadı.",
    L"Video oynatıcı hazır; kayıtlı arka plan başlatılıyor.",
    L"Video oynatıcı hazırlanamadı; VLC kurulumunu kontrol edin.",
    L"VLC oynatıcı kitaplığı bulunamadı veya başlatılamadı. Windows sürümünüze uygun VLC kurulumunun bulunduğunu ve mimarisinin uygulamayla aynı (32/64 bit) olduğunu kontrol edin.",
    L"Kaydedilmiş video bulunamadı; bilgisayarınızdan yeni bir dosya seçin.",
    L"Uygulama ayar klasörü oluşturulamadı.",
    L"Windows masaüstü simge penceresi veya uygun WorkerW katmanı bulunamadı. Windows Gezgini'nin masaüstü kabuğunu yeniden başlatıp tekrar deneyin.",
    L"Masaüstü arka plan penceresi oluşturulamadı.",
    L"Windows pencereleri oluşturulamadı.",
    L"Uygulama dil dosyası yüklenemedi; Türkçe kullanılacak."
};

static wchar_t g_texts[TXT_COUNT][512];
static wchar_t g_language_code[16] = L"tr";
typedef struct libvlc_instance_t libvlc_instance_t;
typedef struct libvlc_media_t libvlc_media_t;
typedef struct libvlc_media_player_t libvlc_media_player_t;

typedef libvlc_instance_t *(__cdecl *fn_libvlc_new)(int, const char *const *);
typedef void (__cdecl *fn_libvlc_release)(libvlc_instance_t *);
typedef libvlc_media_t *(__cdecl *fn_libvlc_media_new_path)(
    libvlc_instance_t *, const char *);
typedef void (__cdecl *fn_libvlc_media_add_option)(libvlc_media_t *, const char *);
typedef void (__cdecl *fn_libvlc_media_release)(libvlc_media_t *);
typedef libvlc_media_player_t *(__cdecl *fn_libvlc_media_player_new)(
    libvlc_instance_t *);
typedef void (__cdecl *fn_libvlc_media_player_release)(libvlc_media_player_t *);
typedef void (__cdecl *fn_libvlc_media_player_set_media)(
    libvlc_media_player_t *, libvlc_media_t *);
typedef void (__cdecl *fn_libvlc_media_player_set_hwnd)(
    libvlc_media_player_t *, void *);
typedef int (__cdecl *fn_libvlc_media_player_play)(libvlc_media_player_t *);
typedef void (__cdecl *fn_libvlc_media_player_stop)(libvlc_media_player_t *);
typedef void (__cdecl *fn_libvlc_audio_set_mute)(libvlc_media_player_t *, int);
typedef void (__cdecl *fn_libvlc_video_set_scale)(
    libvlc_media_player_t *, float);

typedef struct {
    HMODULE library;
    libvlc_instance_t *instance;
    libvlc_media_player_t *player;
    libvlc_media_player_t *preview_player;
    fn_libvlc_release release_instance;
    fn_libvlc_new new_instance;
    fn_libvlc_media_new_path media_new_path;
    fn_libvlc_media_add_option media_add_option;
    fn_libvlc_media_release media_release;
    fn_libvlc_media_player_new player_new;
    fn_libvlc_media_player_release player_release;
    fn_libvlc_media_player_set_media player_set_media;
    fn_libvlc_media_player_set_hwnd player_set_hwnd;
    fn_libvlc_media_player_play player_play;
    fn_libvlc_media_player_stop player_stop;
    fn_libvlc_audio_set_mute audio_set_mute;
    fn_libvlc_video_set_scale video_set_scale;
} Vlc;

typedef struct {
    libvlc_media_player_t *player;
    fn_libvlc_media_player_stop stop;
    fn_libvlc_media_player_release release;
} VlcCleanupTask;

static HINSTANCE g_instance;
static HWND g_main_window;
static HWND g_settings_window;
static HWND g_about_window;
static HWND g_wallpaper_window;
static HWND g_preview_window;
static HWND g_path_edit;
static HWND g_status;
static HWND g_settings_autostart_check;
static HWND g_browse_button;
static HWND g_apply_button;
static HWND g_clear_button;
static HWND g_settings_button;
static HFONT g_about_link_font;
static HFONT g_about_copyright_font;
static HBITMAP g_about_logo_bitmap;
static WNDPROC g_about_link_previous_proc;
static HICON g_app_icon_small;
static HICON g_app_icon_large;
static BOOL g_app_icon_small_owned;
static BOOL g_app_icon_large_owned;
static HANDLE g_vlc_cleanup_event;
static LONG g_vlc_cleanup_count;
static HANDLE g_single_instance_mutex;
static HANDLE g_vlc_prewarm_thread;
static volatile LONG g_vlc_prewarm_state;
static BOOL g_startup_apply_pending;
static wchar_t g_settings_path[MAX_PATH];
static Vlc g_vlc;
static wchar_t g_video_path[32768];
static wchar_t g_preview_source[32768];
static wchar_t g_os_name[96];
static NOTIFYICONDATAW g_tray_icon;
static BOOL g_tray_available;
static UINT g_taskbar_created_message;
static DWORD g_os_major_version;
static DWORD g_os_minor_version;
static UINT g_file_caching_ms = 300;

static DWORD WINAPI vlc_cleanup_thread_proc(void *parameter)
{
    VlcCleanupTask *task = (VlcCleanupTask *)parameter;
    task->stop(task->player);
    task->release(task->player);
    free(task);
    if (InterlockedDecrement(&g_vlc_cleanup_count) == 0)
        SetEvent(g_vlc_cleanup_event);
    return 0;
}

static void release_player_async(libvlc_media_player_t **player_slot)
{
    if (!player_slot || !*player_slot)
        return;

    libvlc_media_player_t *player = *player_slot;
    *player_slot = NULL;
    VlcCleanupTask *task = (VlcCleanupTask *)malloc(sizeof(*task));
    if (!task) {
        g_vlc.player_stop(player);
        g_vlc.player_release(player);
        return;
    }
    task->player = player;
    task->stop = g_vlc.player_stop;
    task->release = g_vlc.player_release;
    if (InterlockedIncrement(&g_vlc_cleanup_count) == 1)
        ResetEvent(g_vlc_cleanup_event);

    HANDLE thread = CreateThread(
        NULL, 0, vlc_cleanup_thread_proc, task, 0, NULL);
    if (thread) {
        CloseHandle(thread);
        return;
    }

    free(task);
    if (InterlockedDecrement(&g_vlc_cleanup_count) == 0)
        SetEvent(g_vlc_cleanup_event);
    g_vlc.player_stop(player);
    g_vlc.player_release(player);
}

static void load_app_icons(void)
{
    g_app_icon_small = (HICON)LoadImageW(
        g_instance, MAKEINTRESOURCEW(IDI_APP_LOGO), IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
        LR_DEFAULTCOLOR);
    g_app_icon_large = (HICON)LoadImageW(
        g_instance, MAKEINTRESOURCEW(IDI_APP_LOGO), IMAGE_ICON,
        GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON),
        LR_DEFAULTCOLOR);

    g_app_icon_small_owned = g_app_icon_small != NULL;
    g_app_icon_large_owned = g_app_icon_large != NULL;
    if (!g_app_icon_small)
        g_app_icon_small = LoadIconW(NULL, IDI_APPLICATION);
    if (!g_app_icon_large)
        g_app_icon_large = LoadIconW(NULL, IDI_APPLICATION);
}

static void release_app_icons(void)
{
    if (g_app_icon_small_owned)
        DestroyIcon(g_app_icon_small);
    if (g_app_icon_large_owned)
        DestroyIcon(g_app_icon_large);
    g_app_icon_small = NULL;
    g_app_icon_large = NULL;
    g_app_icon_small_owned = FALSE;
    g_app_icon_large_owned = FALSE;
}

static BOOL load_settings_path(void)
{
    wchar_t appdata[MAX_PATH];
    DWORD length = GetEnvironmentVariableW(L"APPDATA", appdata, MAX_PATH);
    if (length == 0 || length >= MAX_PATH)
        return FALSE;

    wchar_t directory[MAX_PATH];
    if (swprintf_s(directory, MAX_PATH, L"%s\\%s", appdata, APP_NAME) < 0)
        return FALSE;
    if (!CreateDirectoryW(directory, NULL) &&
        GetLastError() != ERROR_ALREADY_EXISTS)
        return FALSE;
    return swprintf_s(g_settings_path, MAX_PATH, L"%s\\settings.ini",
                      directory) >= 0;
}

static const wchar_t *app_text(TextId id)
{
    return (unsigned)id < TXT_COUNT ? g_texts[id] : L"";
}

static int find_text_key(const char *key)
{
    for (int i = 0; i < TXT_COUNT; ++i) {
        if (strcmp(key, g_text_keys[i]) == 0)
            return i;
    }
    return -1;
}

static BOOL is_supported_language(const wchar_t *code)
{
    static const wchar_t *const codes[] = {
        L"tr", L"en", L"de", L"ar", L"fa", L"fr", L"ru", L"zh-CN",
        L"ja", L"pt", L"es", L"it", L"nl", L"pl", L"uk", L"el",
        L"he", L"ko", L"hi", L"id", L"vi", L"th", L"sv", L"no",
        L"da", L"fi", L"cs", L"ro", L"hu", L"az"
    };

    for (size_t i = 0; i < _countof(codes); ++i) {
        if (_wcsicmp(code, codes[i]) == 0)
            return TRUE;
    }
    return FALSE;
}

static BOOL load_app_language(void)
{
    for (int i = 0; i < TXT_COUNT; ++i) {
        if (wcscpy_s(g_texts[i], _countof(g_texts[i]),
                     g_default_texts[i]) != 0)
            return FALSE;
    }

    HKEY key;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\StarWallpaper",
                      0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
        return TRUE;

    wchar_t selected_language[16] = L"tr";
    DWORD type = 0;
    DWORD size = sizeof(selected_language);
    LONG result = RegQueryValueExW(
        key, L"Language", NULL, &type,
        (BYTE *)selected_language, &size);
    RegCloseKey(key);
    if (result == ERROR_FILE_NOT_FOUND)
        return TRUE;
    if (result != ERROR_SUCCESS || type != REG_SZ ||
        !is_supported_language(selected_language))
        return FALSE;
    if (wcscpy_s(g_language_code, _countof(g_language_code),
                 selected_language) != 0)
        return FALSE;
    if (_wcsicmp(g_language_code, L"tr") == 0)
        return TRUE;

    wchar_t executable[MAX_PATH];
    DWORD path_length = GetModuleFileNameW(
        NULL, executable, _countof(executable));
    if (path_length == 0 || path_length >= _countof(executable))
        return FALSE;
    wchar_t *slash = wcsrchr(executable, L'\\');
    if (!slash)
        return FALSE;
    *slash = L'\0';

    wchar_t translation_path[MAX_PATH];
    if (swprintf_s(translation_path, _countof(translation_path),
                   L"%s\\languages\\app.ini", executable) < 0)
        return FALSE;

    FILE *translation_file = _wfopen(translation_path, L"rb");
    if (!translation_file)
        return FALSE;

    char language_code[16];
    if (WideCharToMultiByte(CP_UTF8, 0, g_language_code, -1,
                            language_code, sizeof(language_code),
                            NULL, NULL) == 0) {
        fclose(translation_file);
        return FALSE;
    }

    BOOL selected_section = FALSE;
    BOOL found[TXT_COUNT] = {0};
    char line[2048];
    if (!fgets(line, sizeof(line), translation_file)) {
        fclose(translation_file);
        return FALSE;
    }
    rewind(translation_file);
    BOOL first_line = TRUE;
    while (fgets(line, sizeof(line), translation_file)) {
        char *start = line;
        if (first_line && (unsigned char)start[0] == 0xEF &&
            (unsigned char)start[1] == 0xBB &&
            (unsigned char)start[2] == 0xBF)
            start += 3;
        first_line = FALSE;
        while (*start && isspace((unsigned char)*start))
            ++start;
        size_t length = strlen(start);
        while (length > 0 &&
               isspace((unsigned char)start[length - 1]))
            start[--length] = '\0';
        if (length == 0 || *start == ';' || *start == '#')
            continue;

        if (*start == '[' && length > 2 && start[length - 1] == ']') {
            start[length - 1] = '\0';
            selected_section = strcmp(start + 1, language_code) == 0;
            continue;
        }
        if (!selected_section)
            continue;

        char *separator = strchr(start, '=');
        if (!separator)
            continue;
        *separator++ = '\0';
        while (*separator && isspace((unsigned char)*separator))
            ++separator;
        int text_id = find_text_key(start);
        if (text_id < 0)
            continue;

        int converted = MultiByteToWideChar(
            CP_UTF8, 0, separator, -1, g_texts[text_id],
            (int)_countof(g_texts[text_id]));
        if (converted == 0) {
            fclose(translation_file);
            return FALSE;
        }
        found[text_id] = TRUE;
    }
    BOOL read_error = ferror(translation_file) != 0;
    fclose(translation_file);
    if (read_error)
        return FALSE;
    for (int i = 0; i < TXT_COUNT; ++i) {
        if (!found[i])
            return FALSE;
    }
    return TRUE;
}

static void enable_dpi_awareness(void)
{
    typedef BOOL (WINAPI *set_process_dpi_awareness_context_fn)(HANDLE);
    typedef BOOL (WINAPI *set_process_dpi_aware_fn)(void);
    HMODULE user32 = GetModuleHandleW(L"user32.dll");
    set_process_dpi_awareness_context_fn set_context = user32
        ? (set_process_dpi_awareness_context_fn)GetProcAddress(
              user32, "SetProcessDpiAwarenessContext")
        : NULL;
    set_process_dpi_aware_fn set_aware = user32
        ? (set_process_dpi_aware_fn)GetProcAddress(
              user32, "SetProcessDPIAware")
        : NULL;

    if (set_context &&
        set_context((HANDLE)(LONG_PTR)-4))
        return;
    if (set_aware)
        set_aware();
}

static void detect_windows_version(void)
{
    typedef LONG (WINAPI *rtl_get_version_fn)(OSVERSIONINFOW *);
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    rtl_get_version_fn rtl_get_version = ntdll
        ? (rtl_get_version_fn)GetProcAddress(ntdll, "RtlGetVersion") : NULL;
    OSVERSIONINFOW version = {0};
    version.dwOSVersionInfoSize = sizeof(version);

    if (!rtl_get_version || rtl_get_version(&version) != 0) {
        wcscpy_s(g_os_name, _countof(g_os_name), L"Windows (version unknown)");
    } else {
        g_os_major_version = version.dwMajorVersion;
        g_os_minor_version = version.dwMinorVersion;
    }

    if (!rtl_get_version || !g_os_major_version) {
        return;
    } else if (version.dwMajorVersion == 10 && version.dwBuildNumber >= 22000) {
        wcscpy_s(g_os_name, _countof(g_os_name), L"Windows 11");
    } else if (version.dwMajorVersion == 10) {
        wcscpy_s(g_os_name, _countof(g_os_name), L"Windows 10");
    } else if (version.dwMajorVersion == 6 && version.dwMinorVersion == 3) {
        wcscpy_s(g_os_name, _countof(g_os_name), L"Windows 8.1");
    } else if (version.dwMajorVersion == 6 && version.dwMinorVersion == 2) {
        wcscpy_s(g_os_name, _countof(g_os_name), L"Windows 8");
    } else if (version.dwMajorVersion == 6 && version.dwMinorVersion == 1) {
        wcscpy_s(g_os_name, _countof(g_os_name), L"Windows 7");
    } else if (version.dwMajorVersion == 5 && version.dwMinorVersion == 1) {
        wcscpy_s(g_os_name, _countof(g_os_name), L"Windows XP");
    } else {
        swprintf_s(g_os_name, _countof(g_os_name), L"Windows %lu.%lu",
                   version.dwMajorVersion, version.dwMinorVersion);
    }
}

static void detect_playback_profile(void)
{
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);

    MEMORYSTATUSEX memory = {0};
    memory.dwLength = sizeof(memory);
    if (!GlobalMemoryStatusEx(&memory))
        return;

    ULONGLONG total_ram_mb = memory.ullTotalPhys / (1024 * 1024);
    ULONGLONG available_ram_mb = memory.ullAvailPhys / (1024 * 1024);
    DWORD processors = system_info.dwNumberOfProcessors;
    BOOL legacy_windows = g_os_major_version == 5 ||
        (g_os_major_version == 6 && g_os_minor_version <= 1);

    if (total_ram_mb <= 4096 || available_ram_mb <= 1024 ||
        processors <= 2 || legacy_windows) {
        g_file_caching_ms = 700;
    } else if (total_ram_mb <= 8192 || available_ram_mb <= 2048 ||
               processors <= 4) {
        g_file_caching_ms = 500;
    } else {
        g_file_caching_ms = 300;
    }
}

static BOOL set_autostart(BOOL enabled)
{
    HKEY key;
    LONG result = RegCreateKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, NULL, 0, KEY_SET_VALUE, NULL, &key, NULL);
    if (result != ERROR_SUCCESS)
        return FALSE;

    BOOL success = TRUE;
    if (enabled) {
        wchar_t executable[MAX_PATH];
        wchar_t command[MAX_PATH + 32];
        DWORD length = GetModuleFileNameW(NULL, executable, _countof(executable));
        if (length == 0 || length >= _countof(executable) ||
            swprintf_s(command, _countof(command), L"\"%s\" --background",
                       executable) < 0) {
            success = FALSE;
        } else {
            result = RegSetValueExW(
                key, APP_NAME, 0, REG_SZ, (const BYTE *)command,
                (DWORD)((wcslen(command) + 1) * sizeof(wchar_t)));
            success = result == ERROR_SUCCESS;
        }
    } else {
        result = RegDeleteValueW(key, APP_NAME);
        success = result == ERROR_SUCCESS || result == ERROR_FILE_NOT_FOUND;
    }

    RegCloseKey(key);
    return success;
}

static BOOL is_autostart_enabled(void)
{
    HKEY key;
    wchar_t value[MAX_PATH + 32];
    DWORD size = sizeof(value);
    DWORD type = 0;
    BOOL enabled = FALSE;

    if (RegOpenKeyExW(
            HKEY_CURRENT_USER,
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
        return FALSE;

    enabled = RegQueryValueExW(
        key, APP_NAME, NULL, &type, (BYTE *)value, &size) == ERROR_SUCCESS &&
        type == REG_SZ;
    RegCloseKey(key);
    return enabled;
}

typedef struct {
    HWND icon_host;
    HWND def_view;
} IconHostSearch;

typedef struct {
    HWND progman;
    HWND icon_host;
    HWND def_view;
    HWND worker;
    HWND wallpaper_worker;
    HWND parent;
    BOOL desktop_shell_found;
} DesktopHost;

typedef struct {
    HWND progman;
    HWND wallpaper_worker;
} WallpaperWorkerSearch;

static BOOL CALLBACK find_wallpaper_worker(HWND hwnd, LPARAM parameter)
{
    WallpaperWorkerSearch *search =
        (WallpaperWorkerSearch *)parameter;
    wchar_t class_name[32];
    if (!GetClassNameW(hwnd, class_name, _countof(class_name)) ||
        wcscmp(class_name, L"WorkerW") != 0)
        return TRUE;

    HWND def_view = FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL);
    if (!def_view) {
        search->wallpaper_worker = hwnd;
        return FALSE;
    }
    return TRUE;
}

static BOOL CALLBACK find_icon_host(HWND hwnd, LPARAM parameter)
{
    IconHostSearch *search = (IconHostSearch *)parameter;
    HWND def_view = FindWindowExW(hwnd, NULL, L"SHELLDLL_DefView", NULL);
    if (def_view) {
        search->icon_host = hwnd;
        search->def_view = def_view;
        return FALSE;
    }
    return TRUE;
}

static DesktopHost find_desktop_host(void)
{
    DesktopHost host = {0};
    host.progman = FindWindowW(L"Progman", NULL);
    host.desktop_shell_found = host.progman != NULL;
    if (!host.progman)
        return host;

    DWORD_PTR result = 0;
    SendMessageTimeoutW(host.progman, 0x052C, 0xD, 0,
                        SMTO_NORMAL, 1000, &result);
    SendMessageTimeoutW(host.progman, 0x052C, 0xD, 1,
                        SMTO_NORMAL, 1000, &result);

    for (int attempt = 0; attempt < 20; ++attempt) {
        host.def_view = FindWindowExW(
            host.progman, NULL, L"SHELLDLL_DefView", NULL);
        if (host.def_view) {
            host.icon_host = host.progman;
            host.worker = FindWindowExW(
                host.progman, NULL, L"WorkerW", NULL);
            WallpaperWorkerSearch search = {host.progman, NULL};
            EnumChildWindows(host.progman, find_wallpaper_worker,
                             (LPARAM)&search);
            host.wallpaper_worker = search.wallpaper_worker;
            host.parent = host.wallpaper_worker
                ? host.wallpaper_worker : host.progman;
            return host;
        }

        IconHostSearch search = {0};
        EnumWindows(find_icon_host, (LPARAM)&search);
        if (search.def_view) {
            host.icon_host = search.icon_host;
            host.def_view = search.def_view;
            host.worker = FindWindowExW(
                NULL, host.icon_host, L"WorkerW", NULL);
            if (host.worker) {
                host.parent = host.worker;
                return host;
            }
        }

        if (attempt == 0) {
            SendMessageTimeoutW(host.progman, 0x052C, 0, 0,
                                SMTO_NORMAL, 1000, &result);
        }
        Sleep(100);
    }
    return host;
}

static BOOL load_vlc_library(void)
{
    wchar_t path[MAX_PATH];
    wchar_t plugin_path[MAX_PATH];
    wchar_t program_files[MAX_PATH];
    DWORD length;

    length = GetEnvironmentVariableW(L"ProgramW6432", program_files, MAX_PATH);
    if (length == 0 || length >= MAX_PATH) {
        length = GetEnvironmentVariableW(L"ProgramFiles", program_files, MAX_PATH);
        if (length == 0 || length >= MAX_PATH)
            return FALSE;
    }
    if (swprintf_s(path, MAX_PATH, L"%s\\VideoLAN\\VLC\\libvlc.dll",
                   program_files) >= 0)
        g_vlc.library = LoadLibraryExW(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (!g_vlc.library) {
        length = GetEnvironmentVariableW(L"ProgramFiles(x86)", program_files, MAX_PATH);
        if (length > 0 && length < MAX_PATH &&
            swprintf_s(path, MAX_PATH, L"%s\\VideoLAN\\VLC\\libvlc.dll",
                       program_files) >= 0)
            g_vlc.library = LoadLibraryExW(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    }
    if (!g_vlc.library)
        return FALSE;

    if (GetModuleFileNameW(g_vlc.library, path, _countof(path)) > 0) {
        wchar_t *slash = wcsrchr(path, L'\\');
        if (slash) {
            *slash = L'\0';
            if (swprintf_s(plugin_path, _countof(plugin_path),
                           L"%s\\plugins", path) >= 0)
                SetEnvironmentVariableW(L"VLC_PLUGIN_PATH", plugin_path);
        }
    }

    g_vlc.new_instance = (fn_libvlc_new)GetProcAddress(
        g_vlc.library, "libvlc_new");
    g_vlc.release_instance = (fn_libvlc_release)GetProcAddress(
        g_vlc.library, "libvlc_release");
    g_vlc.media_new_path = (fn_libvlc_media_new_path)GetProcAddress(
        g_vlc.library, "libvlc_media_new_path");
    g_vlc.media_add_option = (fn_libvlc_media_add_option)GetProcAddress(
        g_vlc.library, "libvlc_media_add_option");
    g_vlc.media_release = (fn_libvlc_media_release)GetProcAddress(
        g_vlc.library, "libvlc_media_release");
    g_vlc.player_new = (fn_libvlc_media_player_new)GetProcAddress(
        g_vlc.library, "libvlc_media_player_new");
    g_vlc.player_release = (fn_libvlc_media_player_release)GetProcAddress(
        g_vlc.library, "libvlc_media_player_release");
    g_vlc.player_set_media = (fn_libvlc_media_player_set_media)GetProcAddress(
        g_vlc.library, "libvlc_media_player_set_media");
    g_vlc.player_set_hwnd = (fn_libvlc_media_player_set_hwnd)GetProcAddress(
        g_vlc.library, "libvlc_media_player_set_hwnd");
    g_vlc.player_play = (fn_libvlc_media_player_play)GetProcAddress(
        g_vlc.library, "libvlc_media_player_play");
    g_vlc.player_stop = (fn_libvlc_media_player_stop)GetProcAddress(
        g_vlc.library, "libvlc_media_player_stop");
    g_vlc.audio_set_mute = (fn_libvlc_audio_set_mute)GetProcAddress(
        g_vlc.library, "libvlc_audio_set_mute");
    g_vlc.video_set_scale = (fn_libvlc_video_set_scale)GetProcAddress(
        g_vlc.library, "libvlc_video_set_scale");

    return g_vlc.new_instance && g_vlc.release_instance && g_vlc.media_new_path &&
        g_vlc.media_add_option &&
        g_vlc.media_release && g_vlc.player_new &&
        g_vlc.player_release && g_vlc.player_set_media && g_vlc.player_set_hwnd &&
        g_vlc.player_play && g_vlc.player_stop &&
        g_vlc.audio_set_mute;
}

static char *wide_to_utf8(const wchar_t *text)
{
    int bytes = WideCharToMultiByte(CP_UTF8, 0, text, -1,
                                    NULL, 0, NULL, NULL);
    if (bytes <= 0)
        return NULL;
    char *result = (char *)malloc((size_t)bytes);
    if (!result)
        return NULL;
    if (!WideCharToMultiByte(CP_UTF8, 0, text, -1,
                             result, bytes, NULL, NULL)) {
        free(result);
        return NULL;
    }
    return result;
}

static void set_source_controls(const wchar_t *source)
{
    SetWindowTextW(g_path_edit, source);
}

static void get_source_from_controls(wchar_t *source, size_t capacity)
{
    if (g_path_edit)
        GetWindowTextW(g_path_edit, source, (int)capacity);
}

static BOOL path_exists(const wchar_t *path)
{
    DWORD attributes = GetFileAttributesW(path);
    return attributes != INVALID_FILE_ATTRIBUTES &&
           !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

static BOOL start_vlc(void)
{
    if (!g_vlc.library && !load_vlc_library())
        return FALSE;
    if (!g_vlc.new_instance)
        return FALSE;
    if (g_vlc.instance)
        return TRUE;

    char file_caching_option[32];
    if (snprintf(file_caching_option, sizeof(file_caching_option),
                 "--file-caching=%u", g_file_caching_ms) < 0)
        return FALSE;

    const char *arguments[] = {
        "--quiet",
        "--no-video-title-show",
        "--no-audio",
        "--avcodec-hw=any",
        file_caching_option
    };
    g_vlc.instance = g_vlc.new_instance((int)_countof(arguments), arguments);
    if (!g_vlc.instance)
        return FALSE;

    return TRUE;
}

static BOOL ensure_wallpaper_player(void)
{
    if (g_vlc.player)
        return TRUE;
    g_vlc.player = g_vlc.player_new(g_vlc.instance);
    if (!g_vlc.player)
        return FALSE;
    g_vlc.player_set_hwnd(g_vlc.player, g_wallpaper_window);
    g_vlc.audio_set_mute(g_vlc.player, 1);
    return TRUE;
}

static void release_preview_player(void)
{
    release_player_async(&g_vlc.preview_player);
    g_preview_source[0] = L'\0';
}

static void release_vlc(void)
{
    if (g_vlc_prewarm_thread) {
        WaitForSingleObject(g_vlc_prewarm_thread, INFINITE);
        CloseHandle(g_vlc_prewarm_thread);
        g_vlc_prewarm_thread = NULL;
    }
    release_preview_player();
    release_player_async(&g_vlc.player);
    if (g_vlc_cleanup_event)
        WaitForSingleObject(g_vlc_cleanup_event, INFINITE);
    if (g_vlc.instance) {
        g_vlc.release_instance(g_vlc.instance);
        g_vlc.instance = NULL;
    }
    if (g_vlc.library) {
        FreeLibrary(g_vlc.library);
        g_vlc.library = NULL;
    }
    if (g_vlc_cleanup_event) {
        CloseHandle(g_vlc_cleanup_event);
        g_vlc_cleanup_event = NULL;
    }
}

static void set_status(const wchar_t *text);

static DWORD WINAPI vlc_prewarm_thread_proc(void *parameter)
{
    (void)parameter;
    BOOL ready = start_vlc();
    InterlockedExchange(&g_vlc_prewarm_state, ready ? 2 : -1);
    if (g_main_window)
        PostMessageW(g_main_window, WM_VLC_PREWARM_COMPLETE, 0, 0);
    return 0;
}

static BOOL start_vlc_prewarm(void)
{
    if (g_vlc.instance)
        return TRUE;
    InterlockedExchange(&g_vlc_prewarm_state, 1);
    g_vlc_prewarm_thread = CreateThread(
        NULL, 0, vlc_prewarm_thread_proc, NULL, 0, NULL);
    if (!g_vlc_prewarm_thread) {
        InterlockedExchange(&g_vlc_prewarm_state, 0);
        return FALSE;
    }
    return TRUE;
}

static void refresh_wallpaper_surface(void)
{
    if (!g_wallpaper_window)
        return;
    InvalidateRect(g_wallpaper_window, NULL, TRUE);
    UpdateWindow(g_wallpaper_window);
    SetWindowPos(g_wallpaper_window, HWND_TOP,
                 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
                 SWP_SHOWWINDOW);
    RedrawWindow(g_wallpaper_window, NULL, NULL,
                 RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

static void add_crop_option(libvlc_media_t *media, HWND target)
{
    if (!target)
        return;

    RECT rect;
    if (!GetClientRect(target, &rect))
        return;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    if (width <= 0 || height <= 0)
        return;

    int a = width;
    int b = height;
    while (b != 0) {
        int remainder = a % b;
        a = b;
        b = remainder;
    }
    if (a <= 0)
        return;

    char option[64];
    if (snprintf(option, sizeof(option), ":crop=%d:%d",
                 width / a, height / a) > 0) {
        g_vlc.media_add_option(media, option);
    }
}

static libvlc_media_t *create_media_for_target(const wchar_t *path,
                                               HWND target)
{
    char *utf8_path = wide_to_utf8(path);
    if (!utf8_path)
        return NULL;

    libvlc_media_t *media = g_vlc.media_new_path(g_vlc.instance, utf8_path);
    free(utf8_path);
    if (!media)
        return NULL;

    char caching_option[48];
    int cache_length = snprintf(caching_option, sizeof(caching_option),
                                ":file-caching=%u", g_file_caching_ms);
    if (cache_length < 0 || (size_t)cache_length >= sizeof(caching_option)) {
        g_vlc.media_release(media);
        return NULL;
    }
    g_vlc.media_add_option(media, caching_option);
    g_vlc.media_add_option(media, ":no-audio");
    g_vlc.media_add_option(media, ":input-repeat=999999");
    add_crop_option(media, target);
    return media;
}

static void start_preview(const wchar_t *path)
{
    if (g_vlc.preview_player &&
        wcscmp(g_preview_source, path) == 0)
        return;
    release_preview_player();
    if (!g_preview_window || !start_vlc()) {
        set_status(app_text(TXT_PREVIEW_START_FAILED));
        return;
    }

    libvlc_media_t *media = create_media_for_target(
        path, g_preview_window);
    if (!media) {
        set_status(app_text(TXT_VIDEO_OPEN_FAILED));
        return;
    }

    if (!g_vlc.preview_player) {
        g_vlc.preview_player = g_vlc.player_new(g_vlc.instance);
        if (!g_vlc.preview_player) {
            g_vlc.media_release(media);
            set_status(app_text(TXT_PREVIEW_CREATE_FAILED));
            return;
        }
        g_vlc.audio_set_mute(g_vlc.preview_player, 1);
    }

    g_vlc.player_set_hwnd(g_vlc.preview_player, g_preview_window);
    g_vlc.player_set_media(g_vlc.preview_player, media);
    g_vlc.media_release(media);

    if (g_vlc.player_play(g_vlc.preview_player) == -1) {
        release_preview_player();
        set_status(app_text(TXT_PREVIEW_PLAY_FAILED));
        return;
    }
    if (g_vlc.video_set_scale)
        g_vlc.video_set_scale(g_vlc.preview_player, 0.0f);
    wcscpy_s(g_preview_source, _countof(g_preview_source), path);
    set_status(app_text(TXT_PREVIEW_READY));
}

static void clear_video(void)
{
    release_preview_player();
    release_player_async(&g_vlc.player);
    if (g_wallpaper_window)
        ShowWindow(g_wallpaper_window, SW_HIDE);

    g_video_path[0] = L'\0';
    set_source_controls(L"");
    if (!WritePrivateProfileStringW(SETTINGS_SECTION, L"VideoPath", NULL,
                                    g_settings_path)) {
        MessageBoxW(g_main_window, app_text(TXT_CLEAR_FAILED),
                    APP_NAME, MB_ICONERROR | MB_OK);
        return;
    }
    set_status(app_text(TXT_VIDEO_CLEARED));
}

static void set_status(const wchar_t *text)
{
    if (g_status)
        SetWindowTextW(g_status, text);
}

static void finish_apply_video(void)
{
    if (g_vlc.video_set_scale)
        g_vlc.video_set_scale(g_vlc.player, 0.0f);
    refresh_wallpaper_surface();
    SetTimer(g_main_window, 1, 500, NULL);

    if (!WritePrivateProfileStringW(SETTINGS_SECTION, L"VideoPath",
                                    g_video_path, g_settings_path)) {
        MessageBoxW(g_main_window, app_text(TXT_SAVE_FAILED),
                    APP_NAME, MB_ICONWARNING | MB_OK);
    }
    set_status(app_text(TXT_VIDEO_APPLIED));
}

static void apply_video(void)
{
    if (InterlockedCompareExchange(&g_vlc_prewarm_state, 0, 0) == 1) {
        g_startup_apply_pending = TRUE;
        set_status(app_text(TXT_APPLY_WAIT));
        return;
    }

    get_source_from_controls(g_video_path, _countof(g_video_path));
    if (g_video_path[0] == L'\0') {
        MessageBoxW(g_main_window, app_text(TXT_CHOOSE_VIDEO),
                    APP_NAME, MB_ICONWARNING | MB_OK);
        return;
    }
    if (!path_exists(g_video_path)) {
        MessageBoxW(g_main_window,
                    app_text(TXT_FILE_MISSING),
                    APP_NAME, MB_ICONERROR | MB_OK);
        return;
    }

    if (!start_vlc()) {
        MessageBoxW(
            g_main_window,
            app_text(TXT_VLC_START_FAILED),
            APP_NAME, MB_ICONERROR | MB_OK);
        return;
    }

    libvlc_media_t *media = create_media_for_target(
        g_video_path, g_wallpaper_window);
    if (!media) {
        MessageBoxW(g_main_window,
                    app_text(TXT_PATH_CONVERT_FAILED),
                    APP_NAME, MB_ICONERROR | MB_OK);
        return;
    }

    release_preview_player();
    release_player_async(&g_vlc.player);
    if (!ensure_wallpaper_player()) {
        g_vlc.media_release(media);
        MessageBoxW(g_main_window, app_text(TXT_PLAYER_CREATE_FAILED),
                    APP_NAME, MB_ICONERROR | MB_OK);
        return;
    }
    g_vlc.player_set_media(g_vlc.player, media);
    g_vlc.media_release(media);
    g_vlc.player_set_hwnd(g_vlc.player, g_wallpaper_window);
    refresh_wallpaper_surface();

    if (g_vlc.player_play(g_vlc.player) == -1) {
        MessageBoxW(
            g_main_window,
            app_text(TXT_PLAYBACK_FAILED),
            APP_NAME, MB_ICONERROR | MB_OK);
        ShowWindow(g_wallpaper_window, SW_HIDE);
        return;
    }

    finish_apply_video();
}

static void choose_video(void)
{
    wchar_t path[32768] = L"";
    OPENFILENAMEW dialog = {0};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = g_main_window;
    wchar_t filter[256] = {0};
    wcscpy_s(filter, _countof(filter), app_text(TXT_VIDEO_FILTER));
    size_t filter_offset = wcslen(filter) + 1;
    wcscpy_s(filter + filter_offset, _countof(filter) - filter_offset,
             L"*.mp4;*.mkv;*.avi;*.mov;*.wmv;*.webm;*.mpeg;*.mpg");
    filter_offset += wcslen(filter + filter_offset) + 1;
    wcscpy_s(filter + filter_offset, _countof(filter) - filter_offset,
             app_text(TXT_ALL_FILES));
    filter_offset += wcslen(filter + filter_offset) + 1;
    wcscpy_s(filter + filter_offset, _countof(filter) - filter_offset,
             L"*.*");
    dialog.lpstrFilter = filter;
    dialog.lpstrFile = path;
    dialog.nMaxFile = (DWORD)_countof(path);
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    dialog.lpstrTitle = app_text(TXT_VIDEO_DIALOG_TITLE);

    if (GetOpenFileNameW(&dialog)) {
        set_source_controls(path);
        wcscpy_s(g_video_path, _countof(g_video_path), path);
        start_preview(path);
    }
}

static void show_main_window(HWND hwnd)
{
    ShowWindow(hwnd, SW_SHOWNORMAL);
    SetForegroundWindow(hwnd);
}

static void show_settings_window(void)
{
    if (!g_settings_window) {
        g_settings_window = CreateWindowExW(
            WS_EX_DLGMODALFRAME, SETTINGS_CLASS, app_text(TXT_SETTINGS_TITLE),
            WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
            430, 150, g_main_window, NULL, g_instance, NULL);
        if (!g_settings_window) {
            MessageBoxW(g_main_window, app_text(TXT_SETTINGS_CREATE_FAILED),
                        APP_NAME, MB_ICONERROR | MB_OK);
            return;
        }
    }
    if (g_settings_window) {
        ShowWindow(g_settings_window, SW_SHOWNORMAL);
        SetForegroundWindow(g_settings_window);
    }
}

static void show_about_window(void)
{
    if (!g_about_window) {
        g_about_window = CreateWindowExW(
            WS_EX_DLGMODALFRAME, ABOUT_CLASS, app_text(TXT_ABOUT_TITLE),
            WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
            600, 390, g_main_window, NULL, g_instance, NULL);
        if (!g_about_window) {
            MessageBoxW(g_main_window, app_text(TXT_ABOUT_CREATE_FAILED),
                        APP_NAME, MB_ICONERROR | MB_OK);
            return;
        }
    }
    ShowWindow(g_about_window, SW_SHOWNORMAL);
    SetForegroundWindow(g_about_window);
}

static void show_tray_menu(HWND hwnd)
{
    HMENU menu = CreatePopupMenu();
    if (!menu)
        return;

    AppendMenuW(menu, MF_STRING, ID_TRAY_OPEN, app_text(TXT_TRAY_OPEN));
    AppendMenuW(menu, MF_STRING, ID_SETTINGS, app_text(TXT_TRAY_VIDEO));
    AppendMenuW(menu, MF_STRING, ID_ABOUT, app_text(TXT_TRAY_ABOUT));
    AppendMenuW(menu, MF_STRING, ID_TRAY_EXIT, app_text(TXT_TRAY_EXIT));

    POINT point;
    GetCursorPos(&point);
    SetForegroundWindow(hwnd);
    UINT command = TrackPopupMenu(
        menu, TPM_RETURNCMD | TPM_RIGHTBUTTON,
        point.x, point.y, 0, hwnd, NULL);
    DestroyMenu(menu);
    PostMessageW(hwnd, WM_NULL, 0, 0);

    if (command == ID_TRAY_OPEN)
        show_main_window(hwnd);
    else if (command == ID_SETTINGS)
        show_settings_window();
    else if (command == ID_ABOUT)
        show_about_window();
    else if (command == ID_TRAY_EXIT)
        SendMessageW(hwnd, WM_COMMAND, ID_TRAY_EXIT, 0);
}

static LRESULT CALLBACK about_link_proc(HWND hwnd, UINT message,
                                        WPARAM wparam, LPARAM lparam)
{
    if (message == WM_SETCURSOR && LOWORD(lparam) == HTCLIENT) {
        SetCursor(LoadCursorW(NULL, IDC_HAND));
        return TRUE;
    }

    WNDPROC previous_proc = g_about_link_previous_proc;
    if (message == WM_NCDESTROY)
        g_about_link_previous_proc = NULL;
    return CallWindowProcW(previous_proc, hwnd, message, wparam, lparam);
}

static LRESULT CALLBACK wallpaper_proc(HWND hwnd, UINT message,
                                      WPARAM wparam, LPARAM lparam)
{
    if (message == WM_ERASEBKGND) {
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect((HDC)wparam, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        return 1;
    }
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

static LRESULT CALLBACK settings_proc(HWND hwnd, UINT message,
                                      WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_CREATE: {
        HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        g_settings_autostart_check = CreateWindowW(
            L"BUTTON", app_text(TXT_AUTOSTART),
            WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
            18, 18, 360, 24, hwnd,
            (HMENU)(INT_PTR)ID_SETTINGS_AUTOSTART, g_instance, NULL);
        HWND save = CreateWindowW(
            L"BUTTON", app_text(TXT_SAVE),
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            220, 60, 85, 28, hwnd,
            (HMENU)(INT_PTR)ID_SETTINGS_SAVE, g_instance, NULL);
        HWND cancel = CreateWindowW(
            L"BUTTON", app_text(TXT_CANCEL), WS_CHILD | WS_VISIBLE,
            315, 60, 85, 28, hwnd,
            (HMENU)(INT_PTR)ID_SETTINGS_CANCEL, g_instance, NULL);
        HWND controls[] = {g_settings_autostart_check, save, cancel};
        for (size_t i = 0; i < _countof(controls); ++i)
            SendMessageW(controls[i], WM_SETFONT, (WPARAM)font, TRUE);
        SendMessageW(g_settings_autostart_check, BM_SETCHECK,
                     is_autostart_enabled() ? BST_CHECKED : BST_UNCHECKED, 0);
        return 0;
    }
    case WM_COMMAND:
        if (LOWORD(wparam) == ID_SETTINGS_CANCEL) {
            DestroyWindow(hwnd);
            return 0;
        }
        if (LOWORD(wparam) == ID_SETTINGS_SAVE) {
            BOOL auto_start = SendMessageW(
                g_settings_autostart_check, BM_GETCHECK, 0, 0) == BST_CHECKED;
            if (!set_autostart(auto_start)) {
                MessageBoxW(hwnd, app_text(TXT_SETTINGS_SAVE_FAILED),
                            APP_NAME, MB_ICONERROR | MB_OK);
                return 0;
            }
            DestroyWindow(hwnd);
            return 0;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        if (g_settings_window == hwnd)
            g_settings_window = NULL;
        return 0;
    }
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

static LRESULT CALLBACK about_proc(HWND hwnd, UINT message,
                                   WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_CREATE: {
        HFONT default_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        LOGFONTW link_log_font = {0};
        if (GetObjectW(default_font, sizeof(link_log_font), &link_log_font)) {
            link_log_font.lfUnderline = TRUE;
            g_about_link_font = CreateFontIndirectW(&link_log_font);
        }

        g_about_logo_bitmap = (HBITMAP)LoadImageW(
            g_instance, MAKEINTRESOURCEW(IDB_ABOUT_LOGO), IMAGE_BITMAP,
            0, 0, LR_CREATEDIBSECTION);
        HWND logo = CreateWindowW(
            L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_BITMAP,
            30, 12, 520, 231, hwnd, NULL, g_instance, NULL);
        if (!g_about_logo_bitmap || !logo) {
            MessageBoxW(hwnd, app_text(TXT_ABOUT_LOGO_FAILED),
                        APP_NAME, MB_ICONERROR | MB_OK);
            if (logo)
                DestroyWindow(logo);
            if (g_about_logo_bitmap) {
                DeleteObject(g_about_logo_bitmap);
                g_about_logo_bitmap = NULL;
            }
            if (g_about_link_font) {
                DeleteObject(g_about_link_font);
                g_about_link_font = NULL;
            }
            return -1;
        }
        SendMessageW(logo, STM_SETIMAGE, IMAGE_BITMAP,
                     (LPARAM)g_about_logo_bitmap);

        HWND link = CreateWindowW(
            L"STATIC", L"TurkHackTeam.Org",
            WS_CHILD | WS_VISIBLE | SS_NOTIFY | SS_CENTER,
            150, 260, 300, 26, hwnd, (HMENU)(INT_PTR)ID_ABOUT_LINK,
            g_instance, NULL);
        HWND credit = CreateWindowW(
            L"STATIC", L"Prepared & Developed by: @ATK",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            150, 298, 300, 24, hwnd,
            (HMENU)(INT_PTR)ID_ABOUT_CREDIT, g_instance, NULL);
        HWND copyright = CreateWindowW(
            L"STATIC", L"\x00A9 2026 Tüm hakları saklıdır.",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            100, 326, 400, 24, hwnd,
            (HMENU)(INT_PTR)ID_ABOUT_COPYRIGHT, g_instance, NULL);
        HWND version = CreateWindowW(
            L"STATIC", app_text(TXT_VERSION),
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            100, 350, 400, 24, hwnd,
            (HMENU)(INT_PTR)ID_ABOUT_VERSION, g_instance, NULL);
        LOGFONTW copyright_log_font = {0};
        BOOL copyright_font_ready =
            GetObjectW(default_font, sizeof(copyright_log_font),
                       &copyright_log_font) != 0;
        if (copyright_font_ready) {
            copyright_log_font.lfWeight = FW_BOLD;
            g_about_copyright_font =
                CreateFontIndirectW(&copyright_log_font);
            copyright_font_ready = g_about_copyright_font != NULL;
        }
        if (!link || !credit || !copyright || !version ||
            !copyright_font_ready) {
            MessageBoxW(hwnd, app_text(TXT_ABOUT_LOGO_FAILED),
                        APP_NAME, MB_ICONERROR | MB_OK);
            if (link)
                DestroyWindow(link);
            if (credit)
                DestroyWindow(credit);
            if (copyright)
                DestroyWindow(copyright);
            if (version)
                DestroyWindow(version);
            DeleteObject(g_about_logo_bitmap);
            g_about_logo_bitmap = NULL;
            if (g_about_copyright_font) {
                DeleteObject(g_about_copyright_font);
                g_about_copyright_font = NULL;
            }
            if (g_about_link_font) {
                DeleteObject(g_about_link_font);
                g_about_link_font = NULL;
            }
            return -1;
        }
        SetLastError(ERROR_SUCCESS);
        g_about_link_previous_proc = (WNDPROC)SetWindowLongPtrW(
            link, GWLP_WNDPROC, (LONG_PTR)about_link_proc);
        if (!g_about_link_previous_proc) {
            MessageBoxW(hwnd, app_text(TXT_ABOUT_LOGO_FAILED),
                        APP_NAME, MB_ICONERROR | MB_OK);
            DeleteObject(g_about_logo_bitmap);
            g_about_logo_bitmap = NULL;
            DeleteObject(g_about_copyright_font);
            g_about_copyright_font = NULL;
            if (g_about_link_font) {
                DeleteObject(g_about_link_font);
                g_about_link_font = NULL;
            }
            return -1;
        }
        SendMessageW(link, WM_SETFONT,
                     (WPARAM)(g_about_link_font
                                  ? g_about_link_font : default_font), TRUE);
        SendMessageW(credit, WM_SETFONT, (WPARAM)default_font, TRUE);
        SendMessageW(copyright, WM_SETFONT,
                     (WPARAM)g_about_copyright_font, TRUE);
        SendMessageW(version, WM_SETFONT, (WPARAM)default_font, TRUE);
        return 0;
    }
    case WM_COMMAND:
        if (LOWORD(wparam) == ID_ABOUT_LINK &&
            HIWORD(wparam) == STN_CLICKED) {
            HINSTANCE result = ShellExecuteW(
                hwnd, L"open", L"https://turkhackteam.org",
                NULL, NULL, SW_SHOWNORMAL);
            if ((INT_PTR)result <= 32)
                MessageBoxW(hwnd, app_text(TXT_WEBSITE_FAILED),
                            APP_NAME, MB_ICONERROR | MB_OK);
            return 0;
        }
        break;
    case WM_CTLCOLORSTATIC:
        if (GetDlgCtrlID((HWND)lparam) == ID_ABOUT_LINK ||
            GetDlgCtrlID((HWND)lparam) == ID_ABOUT_CREDIT ||
            GetDlgCtrlID((HWND)lparam) == ID_ABOUT_COPYRIGHT ||
            GetDlgCtrlID((HWND)lparam) == ID_ABOUT_VERSION) {
            SetTextColor((HDC)wparam,
                         GetDlgCtrlID((HWND)lparam) == ID_ABOUT_LINK
                             ? RGB(0, 102, 204) : RGB(0, 0, 0));
            SetBkMode((HDC)wparam, TRANSPARENT);
            return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        if (g_about_logo_bitmap) {
            DeleteObject(g_about_logo_bitmap);
            g_about_logo_bitmap = NULL;
        }
        if (g_about_link_font) {
            DeleteObject(g_about_link_font);
            g_about_link_font = NULL;
        }
        if (g_about_copyright_font) {
            DeleteObject(g_about_copyright_font);
            g_about_copyright_font = NULL;
        }
        if (g_about_window == hwnd)
            g_about_window = NULL;
        return 0;
    }
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

static LRESULT CALLBACK main_proc(HWND hwnd, UINT message,
                                  WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_CREATE: {
        HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        HWND label = CreateWindowW(
            L"STATIC", app_text(TXT_OS_LABEL), WS_CHILD | WS_VISIBLE,
            18, 16, 110, 22, hwnd, NULL, g_instance, NULL);
        HWND os = CreateWindowW(
            L"STATIC", g_os_name, WS_CHILD | WS_VISIBLE,
            130, 16, 330, 22, hwnd, NULL, g_instance, NULL);
        HWND info = CreateWindowW(
            L"STATIC", app_text(TXT_PREVIEW_HINT),
            WS_CHILD | WS_VISIBLE, 18, 45, 600, 22,
            hwnd, NULL, g_instance, NULL);
        g_preview_window = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_BLACKRECT,
            18, 72, 600, 270, hwnd, NULL, g_instance, NULL);
        HWND path_label = CreateWindowW(
            L"STATIC", app_text(TXT_VIDEO_LABEL), WS_CHILD | WS_VISIBLE,
            18, 354, 150, 22, hwnd, NULL, g_instance, NULL);
        g_path_edit = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            18, 380, 465, 26, hwnd, (HMENU)(INT_PTR)ID_VIDEO_PATH,
            g_instance, NULL);
        g_browse_button = CreateWindowW(
            L"BUTTON", app_text(TXT_BROWSE),
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            492, 380, 126, 26, hwnd, (HMENU)(INT_PTR)ID_BROWSE,
            g_instance, NULL);
        g_apply_button = CreateWindowW(
            L"BUTTON", app_text(TXT_APPLY),
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            18, 438, 100, 30, hwnd, (HMENU)(INT_PTR)ID_APPLY,
            g_instance, NULL);
        if (InterlockedCompareExchange(&g_vlc_prewarm_state, 0, 0) == 1) {
            EnableWindow(g_browse_button, FALSE);
            EnableWindow(g_apply_button, FALSE);
        }
        g_clear_button = CreateWindowW(
            L"BUTTON", app_text(TXT_CLEAR), WS_CHILD | WS_VISIBLE,
            130, 438, 90, 30, hwnd, (HMENU)(INT_PTR)ID_CLEAR,
            g_instance, NULL);
        g_settings_button = CreateWindowW(
            L"BUTTON", app_text(TXT_SETTINGS), WS_CHILD | WS_VISIBLE,
            232, 438, 100, 30, hwnd, (HMENU)(INT_PTR)ID_SETTINGS,
            g_instance, NULL);
        HWND about = CreateWindowW(
            L"BUTTON", app_text(TXT_ABOUT), WS_CHILD | WS_VISIBLE,
            342, 438, 100, 30, hwnd, (HMENU)(INT_PTR)ID_ABOUT,
            g_instance, NULL);
        g_status = CreateWindowW(
            L"STATIC", app_text(TXT_INITIAL_STATUS),
            WS_CHILD | WS_VISIBLE, 18, 480, 600, 34, hwnd,
            (HMENU)(INT_PTR)ID_STATUS, g_instance, NULL);

        HWND controls[] = {label, os, info, g_preview_window, path_label,
                           g_path_edit, g_browse_button, g_apply_button,
                           g_clear_button, g_settings_button, about, g_status};
        for (size_t i = 0; i < _countof(controls); ++i)
            SendMessageW(controls[i], WM_SETFONT, (WPARAM)font, TRUE);


        set_source_controls(path_exists(g_video_path) ? g_video_path : L"");
        wchar_t profile_status[128];
        swprintf_s(profile_status, _countof(profile_status),
                   app_text(TXT_CACHE_STATUS),
                   g_file_caching_ms);
        set_status(profile_status);

        ZeroMemory(&g_tray_icon, sizeof(g_tray_icon));
        g_tray_icon.cbSize = NOTIFYICONDATA_V2_SIZE;
        g_tray_icon.hWnd = hwnd;
        g_tray_icon.uID = 1;
        g_tray_icon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        g_tray_icon.uCallbackMessage = WM_TRAY_CALLBACK;
        g_tray_icon.hIcon = g_app_icon_small;
        wcscpy_s(g_tray_icon.szTip, _countof(g_tray_icon.szTip),
                 app_text(TXT_TRAY_TIP));
        g_tray_available = Shell_NotifyIconW(
            NIM_ADD, &g_tray_icon) != FALSE;
        if (!g_tray_available)
            set_status(app_text(TXT_TRAY_FAILED));
        return 0;
    }
    case WM_COMMAND:
        switch (LOWORD(wparam)) {
        case ID_BROWSE:
            choose_video();
            return 0;
        case ID_APPLY:
            apply_video();
            return 0;
        case ID_CLEAR:
            clear_video();
            return 0;
        case ID_SETTINGS:
            show_settings_window();
            return 0;
        case ID_ABOUT:
            show_about_window();
            return 0;
        case ID_TRAY_OPEN:
            show_main_window(hwnd);
            return 0;
        case ID_TRAY_EXIT:
            DestroyWindow(hwnd);
            return 0;
        }
        break;
    case WM_SHOW_MAIN_WINDOW:
        show_main_window(hwnd);
        return 0;
    case WM_VLC_PREWARM_COMPLETE: {
        LONG state = InterlockedCompareExchange(&g_vlc_prewarm_state, 0, 0);
        if (g_vlc_prewarm_thread) {
            CloseHandle(g_vlc_prewarm_thread);
            g_vlc_prewarm_thread = NULL;
        }
        EnableWindow(g_browse_button, TRUE);
        EnableWindow(g_apply_button, TRUE);
        if (state == 2) {
            set_status(app_text(TXT_VLC_PREWARM_READY));
            if (g_startup_apply_pending) {
                g_startup_apply_pending = FALSE;
                PostMessageW(hwnd, WM_APPLY_SAVED_VIDEO, 0, 0);
            }
        } else {
            set_status(app_text(TXT_VLC_PREWARM_FAILED));
            if (g_startup_apply_pending) {
                g_startup_apply_pending = FALSE;
                PostMessageW(hwnd, WM_APPLY_SAVED_VIDEO, 0, 0);
            }
        }
        return 0;
    }
    case WM_TRAY_CALLBACK:
        if (wparam == 1) {
            if (lparam == WM_LBUTTONUP)
                show_main_window(hwnd);
            else if (lparam == WM_RBUTTONUP || lparam == WM_CONTEXTMENU)
                show_tray_menu(hwnd);
        }
        return 0;
    case WM_APPLY_SAVED_VIDEO:
        if (InterlockedCompareExchange(&g_vlc_prewarm_state, 0, 0) == 1) {
            g_startup_apply_pending = TRUE;
            return 0;
        }
        if (InterlockedCompareExchange(&g_vlc_prewarm_state, 0, 0) == -1) {
            set_status(app_text(TXT_VLC_PREWARM_FAILED));
            MessageBoxW(
                hwnd,
                app_text(TXT_VLC_INIT_FAILED),
                APP_NAME, MB_ICONERROR | MB_OK);
            return 0;
        }
        if (g_video_path[0] && path_exists(g_video_path))
            apply_video();
        else if (g_video_path[0])
            set_status(app_text(TXT_SAVED_VIDEO_MISSING));
        return 0;
    case WM_CLOSE:
        release_preview_player();
        if (g_tray_available) {
            ShowWindow(hwnd, SW_HIDE);
        } else {
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_UNINSTALL_SHUTDOWN:
        DestroyWindow(hwnd);
        return 0;
    case WM_TIMER:
        if (wparam == 1) {
            KillTimer(hwnd, 1);
            if (g_vlc.video_set_scale) {
                if (g_vlc.player)
                    g_vlc.video_set_scale(g_vlc.player, 0.0f);
                if (g_vlc.preview_player)
                    g_vlc.video_set_scale(g_vlc.preview_player, 0.0f);
            }
            return 0;
        }
        break;
    case WM_DESTROY:
        if (g_tray_available)
            Shell_NotifyIconW(NIM_DELETE, &g_tray_icon);
        release_vlc();
        release_app_icons();
        if (g_wallpaper_window) {
            DestroyWindow(g_wallpaper_window);
            g_wallpaper_window = NULL;
        }
        PostQuitMessage(0);
        return 0;
    default:
        if (g_taskbar_created_message && message == g_taskbar_created_message) {
            g_tray_available = Shell_NotifyIconW(
                NIM_ADD, &g_tray_icon) != FALSE;
            return 0;
        }
        break;
    }
    return DefWindowProcW(hwnd, message, wparam, lparam);
}

static BOOL register_window_classes(void)
{
    WNDCLASSEXW main_class = {0};
    main_class.cbSize = sizeof(main_class);
    main_class.hInstance = g_instance;
    main_class.lpfnWndProc = main_proc;
    main_class.lpszClassName = WINDOW_CLASS;
    main_class.hCursor = LoadCursorW(NULL, IDC_ARROW);
    main_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    main_class.hIcon = g_app_icon_large;
    main_class.hIconSm = g_app_icon_small;
    if (!RegisterClassExW(&main_class))
        return FALSE;

    WNDCLASSEXW wallpaper_class = {0};
    wallpaper_class.cbSize = sizeof(wallpaper_class);
    wallpaper_class.hInstance = g_instance;
    wallpaper_class.lpfnWndProc = wallpaper_proc;
    wallpaper_class.lpszClassName = WALLPAPER_CLASS;
    wallpaper_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    if (!RegisterClassExW(&wallpaper_class))
        return FALSE;

    WNDCLASSEXW settings_class = {0};
    settings_class.cbSize = sizeof(settings_class);
    settings_class.hInstance = g_instance;
    settings_class.lpfnWndProc = settings_proc;
    settings_class.lpszClassName = SETTINGS_CLASS;
    settings_class.hCursor = LoadCursorW(NULL, IDC_ARROW);
    settings_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClassExW(&settings_class))
        return FALSE;

    WNDCLASSEXW about_class = {0};
    about_class.cbSize = sizeof(about_class);
    about_class.hInstance = g_instance;
    about_class.lpfnWndProc = about_proc;
    about_class.lpszClassName = ABOUT_CLASS;
    about_class.hCursor = LoadCursorW(NULL, IDC_ARROW);
    about_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    return RegisterClassExW(&about_class) != 0;
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previous,
                    PWSTR command_line, int show_command)
{
    (void)previous;
    BOOL start_in_background = command_line &&
        wcscmp(command_line, L"--background") == 0;
    g_single_instance_mutex = CreateMutexW(
        NULL, TRUE, L"Local\\StarWallpaper.SingleInstance");
    if (!g_single_instance_mutex) {
        MessageBoxW(NULL, L"Tek uygulama örneği denetimi başlatılamadı.",
                    APP_NAME, MB_ICONERROR | MB_OK);
        return 1;
    }
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (!start_in_background) {
            HWND existing_window = FindWindowW(WINDOW_CLASS, NULL);
            if (existing_window)
                PostMessageW(existing_window, WM_SHOW_MAIN_WINDOW, 0, 0);
        }
        CloseHandle(g_single_instance_mutex);
        g_single_instance_mutex = NULL;
        return 0;
    }

    g_instance = instance;
    if (!load_app_language()) {
        wcscpy_s(g_language_code, _countof(g_language_code), L"tr");
        for (int i = 0; i < TXT_COUNT; ++i)
            wcscpy_s(g_texts[i], _countof(g_texts[i]),
                     g_default_texts[i]);
        MessageBoxW(NULL, app_text(TXT_LANGUAGE_LOAD_FAILED),
                    APP_NAME, MB_ICONWARNING | MB_OK);
    }

    g_vlc_cleanup_event = CreateEventW(NULL, TRUE, TRUE, NULL);
    if (!g_vlc_cleanup_event) {
        MessageBoxW(NULL, app_text(TXT_VLC_INIT_FAILED),
                    APP_NAME, MB_ICONERROR | MB_OK);
        return 1;
    }
    load_app_icons();
    enable_dpi_awareness();
    g_taskbar_created_message = RegisterWindowMessageW(L"TaskbarCreated");
    detect_windows_version();
    detect_playback_profile();

    if (!load_settings_path()) {
        MessageBoxW(NULL, app_text(TXT_SETTINGS_PATH_FAILED),
                    APP_NAME, MB_ICONERROR | MB_OK);
        return 1;
    }
    GetPrivateProfileStringW(SETTINGS_SECTION, L"VideoPath", L"",
                             g_video_path, _countof(g_video_path),
                             g_settings_path);
    if (path_exists(g_video_path) && !start_vlc_prewarm()) {
        InterlockedExchange(&g_vlc_prewarm_state, 0);
    }
    if (!register_window_classes()) {
        release_vlc();
        MessageBoxW(NULL, app_text(TXT_WINDOW_CREATE_FAILED),
                    APP_NAME, MB_ICONERROR | MB_OK);
        return 1;
    }

    DesktopHost desktop = find_desktop_host();
    if (!desktop.parent) {
        const wchar_t *message = app_text(TXT_DESKTOP_HOST_FAILED);
        MessageBoxW(NULL, message, APP_NAME, MB_ICONERROR | MB_OK);
        release_vlc();
        return 1;
    }

    RECT desktop_rect = {0};
    int desktop_x = 0;
    int desktop_y = 0;
    int desktop_width = 0;
    int desktop_height = 0;
    if (GetClientRect(desktop.parent, &desktop_rect)) {
        desktop_width = desktop_rect.right - desktop_rect.left;
        desktop_height = desktop_rect.bottom - desktop_rect.top;
    }
    if (desktop_width <= 0 || desktop_height <= 0) {
        desktop_width = GetSystemMetrics(SM_CXSCREEN);
        desktop_height = GetSystemMetrics(SM_CYSCREEN);
    }

    g_wallpaper_window = CreateWindowExW(
        WS_EX_NOACTIVATE, WALLPAPER_CLASS, APP_NAME,
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        desktop_x, desktop_y, desktop_width, desktop_height,
        desktop.parent, NULL, instance, NULL);
    if (!g_wallpaper_window) {
        MessageBoxW(NULL, app_text(TXT_WALLPAPER_WINDOW_FAILED),
                    APP_NAME, MB_ICONERROR | MB_OK);
        release_vlc();
        return 1;
    }

    HWND insert_after = desktop.parent == desktop.progman && desktop.def_view
        ? desktop.def_view : HWND_TOP;
    SetWindowPos(g_wallpaper_window, insert_after,
                 desktop_x, desktop_y, desktop_width, desktop_height,
                 SWP_NOACTIVATE);

    g_main_window = CreateWindowExW(
        0, WINDOW_CLASS, app_text(TXT_MAIN_TITLE),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 650, 570, NULL, NULL, instance, NULL);
    if (!g_main_window) {
        DestroyWindow(g_wallpaper_window);
        MessageBoxW(NULL, app_text(TXT_WINDOW_CREATE_FAILED),
                    APP_NAME, MB_ICONERROR | MB_OK);
        release_vlc();
        return 1;
    }

    ShowWindow(g_main_window,
               start_in_background && g_tray_available ? SW_HIDE : show_command);
    UpdateWindow(g_main_window);
    PostMessageW(g_main_window, WM_APPLY_SAVED_VIDEO, 0, 0);

    MSG message;
    while (GetMessageW(&message, NULL, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    if (g_single_instance_mutex) {
        CloseHandle(g_single_instance_mutex);
        g_single_instance_mutex = NULL;
    }
    return (int)message.wParam;
}
