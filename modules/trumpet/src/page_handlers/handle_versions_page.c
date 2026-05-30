#include "../../includes/page_handlers/handle_versions_page.h"

#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pdomovoy_common/version.h"
#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"
#include <stdio.h>

void handle_versions_page(SSD1306_Frame* frame) {
    static char trumpet_version_str[10];
    static char warden_version_str[10];

    snprintf(trumpet_version_str, sizeof(trumpet_version_str), "@T %s",
             TRUMPET_VERSION);

    ssd1306_render_string(frame, 0, 16, trumpet_version_str, 1, false);

    if (g_warden_connected) {
        snprintf(warden_version_str, sizeof(warden_version_str), "@W %s",
                 g_warden_version);

        ssd1306_render_string(frame, 0, 28, warden_version_str, 1, false);
    }
}
