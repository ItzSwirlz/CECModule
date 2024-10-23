#include <avm/cec.h>
#include <tve/cec.h>
#include <wums.h>

typedef struct {
  TVECECLogicalAddress initiator;
  TVECECOpCode opCode;
  uint8_t *parameters; // FIXME: could be more than one parameter?
  uint8_t numParameters;
} LatestCECCommand;

LatestCECCommand *latest_command;

// TODO: typedef int for CECModuleStatus
int CECM_Enable() {
  AVMEnableCEC();

  return TVESetCECEnable(true);
}

int CECM_Disable() {
  AVMDisableCEC();

  return TVESetCECEnable(false);
}

bool CECM_GetEnabled() { return TVEIsCECEnable(); }

// Returns TRUE if a new command has been received
int CECM_GetLatestCommand(LatestCECCommand *outCommand) {
  if (!outCommand) {
    return -1;
  }

  int has_changed = 0;
  if (!latest_command) {
    latest_command = new LatestCECCommand();
  }

  // Always returns true, so we need to see if anything got written to it
  TVECECLogicalAddress outInitiator;
  TVECECOpCode outOpCode;
  uint8_t outParams;
  uint8_t outNumParams;
  TVECECReceiveCommand(&outInitiator, &outOpCode, &outParams, &outNumParams);

  if (outInitiator != latest_command->initiator ||
      outOpCode != latest_command->opCode ||
      &outParams != latest_command->parameters ||
      outNumParams != latest_command->numParameters) {
    has_changed = 1;
  }

  latest_command->initiator = outInitiator;
  latest_command->opCode = outOpCode;
  latest_command->parameters = &outParams;
  latest_command->numParameters = outNumParams;

  outCommand = latest_command;
  return has_changed;
}

int CECM_SendCommand(TVECECLogicalAddress destination, TVECECOpCode opCode,
                     uint8_t *params, uint8_t numParams) {
  return TVECECSendCommand(destination, opCode, params, numParams);
}

WUMS_EXPORT_FUNCTION(CECM_Disable);
WUMS_EXPORT_FUNCTION(CECM_Enable);
WUMS_EXPORT_FUNCTION(CECM_GetEnabled);
WUMS_EXPORT_FUNCTION(CECM_SendCommand);
WUMS_EXPORT_FUNCTION(CECM_GetLatestCommand);
