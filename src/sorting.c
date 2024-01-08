//
// Created by miika on 1/8/24.
//

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glib.h>

void scan_directory(const char *dir_path);

GList *file_list = NULL;

char main_folders[6][30] = {
        "Documents",
        "Pictures",
        "Videos",
        "Music",
        "Applications",
        "Other"
};

char documents_subfolders[11][30] = {
        "PDF",
        "DOCX",
        "DOC",
        "TXT",
        "ODT",
        "XLSX",
        "XLS",
        "ODS",
        "PPTX",
        "PPT",
        "ODP"
};

char pictures_subfolders[5][30] = {
        "PNG",
        "JPG",
        "JPEG",
        "GIF",
        "SVG"
};

char videos_subfolders[5][30] = {
        "MP4",
        "MKV",
        "AVI",
        "MOV",
        "WMV"
};

char music_subfolders[5][30] = {
        "MP3",
        "WAV",
        "FLAC",
        "M4A",
        "AAC"
};

char applications_subfolders[5][30] = {
        "EXE",
        "APP",
        "DEB",
        "RPM",
        "DMG"
};

void init_sorter(char *root_folder) {
    printf("Initializing sorter...\n");
    printf("Root folder: %s\n", root_folder);

    scan_directory(root_folder);
}


void scan_directory(const char *dir_path) {
    // Open directory stream
    DIR *dir = opendir(dir_path);

    // Check if the directory stream was opened successfully
    if (dir == NULL) {
        printf("Failed to open directory: %s\n", dir_path);
        return;
    }

    // Iterate over the directory entries
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip the entries "." and ".." as they refer to the current and parent directory respectively
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Construct the full path of the directory entry
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        // Get information about the directory entry
        struct stat path_stat;
        stat(path, &path_stat);

        // Check if the entry is a directory
        if (S_ISDIR(path_stat.st_mode)) {
            // Recursively scan the subdirectory
            scan_directory(path);
        } else {
            // Add the file path to the list
            file_list = g_list_append(file_list, g_strdup(path));
        }
    }
    // Close the directory stream
    closedir(dir);
}