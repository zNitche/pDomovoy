#pragma once

#include "pdomovoy_common/types.h"
#include "types.h"

extern const PageData pages[];

PageData* get_current_page();
void switch_page(enum PageSwitchDirection direction);
