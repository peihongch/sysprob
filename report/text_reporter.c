#include <stdio.h>

#include "text_reporter.h"

static int text_report(Reporter *self, report_data_t *data);

Reporter text_reporter = {
    .format = REPORTER_FORMAT_TEXT,
    .report = text_report
};

static int text_report(Reporter *self, report_data_t *data) {
    int i;

    if (data->title)
        printf("%s: ", data->title);

    for (i = 0; i < data->num_entries; i++) {
        const char *key = data->entries[i].key;
        double value = data->entries[i].value;
        const char *value_suffix = data->entries[i].value_suffix;
        printf("%s: %.2f%s%s", key, value, value_suffix, (i == data->num_entries - 1) ? "\n" : ", ");
    }

    return 0;
}
