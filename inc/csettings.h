#ifndef CSETTINGS_H_GUARD
#define CSETTINGS_H_GUARD

struct settings_data {
    char log_filename[256];
    char statistics_filename[256];
    int period;
};

/* Lê os argumentos do ficheiro args.txt e preenche a info_container */
void read_args_file(const char *filename, struct info_container *info);

/* Lê os settings do ficheiro settings.txt e preenche a estrutura settings_data */
void read_settings_file(const char *filename, struct settings_data *settings);

#endif