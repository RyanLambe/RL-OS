#pragma once

#include "../include/types.h"

namespace Time{

    void tick(Registers* registers);
    void SetupTime(unsigned short frequency);

}