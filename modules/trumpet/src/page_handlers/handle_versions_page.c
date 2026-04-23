#include "../../includes/page_handlers/handle_versions_page.h"

#include "../../includes/types.h"
#include "pdomovoy_common/version.h"
#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"
#include <stdio.h>

void handle_versions_page(SSD1306_Frame* frame) {
    char trumpet_version_str[9];

    snprintf(trumpet_version_str, sizeof(trumpet_version_str), "@T %s",
             TRUMPET_VERSION);

    ssd1306_render_string(frame, 0, 16, trumpet_version_str, 1, false);
}
