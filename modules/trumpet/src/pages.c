#include "../includes/pages.h"

#include "../includes/globals.h"
#include "../includes/types.h"

#include "../includes/page_handlers/handle_home_page.h"
#include "../includes/page_handlers/handle_time_stats_page.h"
#include "../includes/page_handlers/handle_versions_page.h"

const PageData home_page = {
    .id = 0, .title = "pDomovoy", .handler = handle_home_page};

const PageData versions_page = {
    .id = 1, .title = "versions", .handler = handle_versions_page};

const PageData time_stats_page = {
    .id = 2, .title = "time stats", .handler = handle_time_stats_page};

const PageData pages[] = {home_page, time_stats_page, versions_page};

PageData* get_current_page() { return &pages[g_current_page_id]; };

void switch_page(enum PageSwitchDirection direction) {
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
