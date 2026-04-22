#include "../includes/pages.h"

#include "../includes/globals.h"
#include "pdomovoy_common/types.h"

const PageData env_sensors_page = {.title = "Env Sensors"};
const PageData test_1_page = {.title = "Test1"};
const PageData test_2_page = {.title = "Test2"};

const PageData pages[] = {env_sensors_page, test_1_page, test_2_page};

PageData* get_current_page() { return &pages[current_page_id]; };

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
