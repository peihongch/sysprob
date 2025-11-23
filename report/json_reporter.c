#include <stdio.h>

#include "json_reporter.h"

static int json_report(Reporter *self, report_data_t *data);

Reporter json_reporter = {
    .format = REPORTER_FORMAT_JSON,
    .report = json_report
};

static int json_report(Reporter *self, report_data_t *data) {
    int i;

    printf("{\n");
    if (data->title)
        printf("  \"title\": \"%s\",\n", data->title);

    printf("  \"entries\": [\n");
    for (i = 0; i < data->num_entries; i++) {
        const char *key = data->entries[i].key;
        double value = data->entries[i].value;
        const char *value_suffix = data->entries[i].value_suffix;

        printf("    {\n");
        printf("      \"%s\": \"%.2f%s\",\n", key, value, value_suffix);
        printf("    }%s\n", (i == data->num_entries - 1) ? "" : ",");
    }
    printf("  ]\n");
    printf("}\n");

    return 0;
}
