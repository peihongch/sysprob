#include <stdio.h>

#include "csv_reporter.h"

static int csv_report(Reporter *self, report_data_t *data);

Reporter csv_reporter = {
    .format = REPORTER_FORMAT_CSV,
    .report = csv_report
};

static int csv_report(Reporter *self, report_data_t *data) {
    int i;

    if (data->title)
        printf("%s,", data->title);
    for (i = 0; i < data->num_entries; i++) {
        const char *key = data->entries[i].key;
        printf("%s%s", key, (i == data->num_entries - 1) ? "\n" : ",");
    }

    if (data->title)
        printf(",");
    for (i = 0; i < data->num_entries; i++) {
        double value = data->entries[i].value;
        const char *value_suffix = data->entries[i].value_suffix;
        printf("%.2f%s%s", value, value_suffix, (i == data->num_entries - 1) ? "\n" : ",");
    }

    return 0;
}
