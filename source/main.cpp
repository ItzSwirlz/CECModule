#include "utils/logger.h"
#include <avm/cec.h>
#include <coreinit/debug.h>
#include <tve/cec.h>
#include <wums.h>

WUMS_MODULE_EXPORT_NAME("homebrew_cec");
WUMS_MODULE_AUTHOR("ItzSwirlz");
WUMS_MODULE_VERSION("0.1");
WUMS_MODULE_LICENSE("GPL");
WUMS_MODULE_DESCRIPTION("CEC module");

bool cec_initialized;

WUMS_INITIALIZE() {
  initLogging();

  bool tve_cec_initialized = TVECECInit();
  bool avm_cec_initialized = AVMCECInit();

  cec_initialized = tve_cec_initialized && avm_cec_initialized;
}

WUMS_DEINITIALIZE() { deinitLogging(); }
