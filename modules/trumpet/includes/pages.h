#pragma once

#include "pdomovoy_common/types.h"

extern const PageData pages[];

PageData* get_current_page();
void switch_page(int direction);
