#pragma once

#undef __CRASH_RPT__
#define __CRASH_RPT__

#undef __WITH_CONSOLE__
#ifdef _DEBUG
#define __WITH_CONSOLE__
#endif
#undef __WITH_TIBRVCO__

#undef __LIZI__
//#define __LIZI__

#undef __CHECK_SUPER_DOG__
//#define __CHECK_SUPER_DOG__ // 加密狗

#undef __STATE_ELICITATION__
//#define __STATE_ELICITATION__ // 弃用

#undef __READ_CSV__
//#define __READ_CSV__ 开启读取csv文件功能

#define SOFT_VER "  v2.0.0.201012_RC"

static bool GLOBAL_LOCKED = false;
extern bool PLAN_ONE;