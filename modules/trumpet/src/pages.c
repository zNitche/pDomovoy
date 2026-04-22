#include "../includes/pages.h"

#include "../includes/globals.h"
#include "../includes/types.h"

#include "../includes/page_handlers/page_env_sensors.h"

const PageData env_sensors_page = {.title = "sensors",
                                   .handler = handle_env_sensors_page};
const PageData alarm_page = {.title = "alarm", .handler = NULL};
const PageData power_page = {.title = "power", .handler = NULL};

const PageData pages[] = {env_sensors_page, alarm_page, power_page};

PageData* get_current_page() { return &pages[g_current_page_id]; };

// PageSwitchDirection
void switch_page(int direction) {
    const int pages_count = count_of(pages);
    const int max_page_id = pages_count - 1;

    switch (direction) {
    case PAGE_SWITCH_NEXT:
        if (g_current_page_id >= max_page_id) {
            g_current_page_id = 0;
        } else {
            g_current_page_id++;
        }

        break;

    case PAGE_SWITCH_PREVIOUS:
        if (g_current_page_id == 0) {
            g_current_page_id = max_page_id;
        } else {
            g_current_page_id--;
        }

        break;

    default:
        break;
    }
}
