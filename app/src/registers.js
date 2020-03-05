// Registers
const registers = ['RAX', 'RCX', 'RDX', 'RBX', 'RSP', 'RBP', 'RSI', 'RDI', 'R8', 'R9', 'R10', 'R11', 'R12', 'R13', 'R14'];

// Condition code registers
const conditionCodes = ['OF', 'SF', 'ZF'];

// Pipeline registers
const fetchStage = ['PRED_PC'];
const decodeStage = ['STAT', 'I_CODE', 'I_FUN', 'R_A', 'R_B', 'VAL_C', 'VAL_P'];
const executeStage = ['STAT', 'I_CODE', 'I_FUN', 'VAL_C', 'VAL_A', 'VAL_B', 'DST_E', 'DST_M', 'SRC_A', 'SRC_B'];
const memoryStage = ['STAT', 'I_CODE', 'CND', 'VAL_E', 'VAL_A', 'DST_E', 'DST_M'];
const writebackStage = ['STAT', 'I_CODE', 'VAL_E', 'VAL_M', 'DST_E', 'DST_M'];
const pipelineRegisters = [
  { 'title': 'F', 'id': 'fetch', 'registers': fetchStage },
  { 'title': 'D', 'id': 'decode', 'registers': decodeStage },
  { 'title': 'E', 'id': 'execute', 'registers': executeStage },
  { 'title': 'M', 'id': 'memory', 'registers': memoryStage },
  { 'title': 'W', 'id': 'writeback', 'registers': writebackStage }
];

export { registers, conditionCodes, pipelineRegisters };