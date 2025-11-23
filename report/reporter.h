#ifndef REPORTER_H
#define REPORTER_H

typedef enum {
    REPORTER_FORMAT_TEXT,
    REPORTER_FORMAT_CSV,
    REPORTER_FORMAT_JSON,
} reporter_format_t;

typedef struct {
    int num_entries;
    const char *title;
    struct {
        const char *key;
        double value;
        const char *value_suffix;
    } entries[100];
} report_data_t;

typedef struct Reporter {
    reporter_format_t format;
    int (*report)(struct Reporter *self, report_data_t *data);
} Reporter;

Reporter *get_reporter(reporter_format_t format);
int report_data(Reporter *reporter, report_data_t *data);

#endif /* REPORTER_H */
