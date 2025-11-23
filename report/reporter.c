#include <stdio.h>

#include "reporter.h"
#include "text_reporter.h"
#include "csv_reporter.h"
#include "../util/logger.h"

#define MAX_REPORTERS 10

static Reporter *reporters[MAX_REPORTERS] = {
    [REPORTER_FORMAT_TEXT] = &text_reporter,
    [REPORTER_FORMAT_CSV] = &csv_reporter,
};

Reporter *get_reporter(reporter_format_t format) {
    if (format < 0 || format >= MAX_REPORTERS) {
        LOG_ERROR("Invalid reporter format: %d", format);
        return NULL;
    }
    return reporters[format];
}

int report_data(Reporter *reporter, report_data_t *data) {
    if (!reporter || !reporter->report) {
        LOG_ERROR("Invalid reporter or report function");
        return -1;
    }
    return reporter->report(reporter, data);
}
