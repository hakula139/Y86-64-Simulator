'use strict';

import './style.css';
import '../favicon/favicon.ico';

import { registers, conditionCodes, pipelineRegisters } from './registers';
import { outputResult } from './output';

const $ = mdui.JQ;

/* Selectors */

const body = $('body');
const progress = $('#progress');
const previous = $('#previous');
const run = $('#run');
const next = $('#next');
const restart = $('#restart');
const speed = $('#speed');
const controllers = $('.controller');
const displayMode = $('#display_mode');
const base = $('#base');
const clock = $('#clock');
const instruction = $('#instruction');
const cpi = $('#cpi');
let values;
let icode;

/* Templates */

const registerTemplate = $('#register_template').html();
const pipelineTemplate = $('#pipeline_template').html();

/* Initialization */

function appendRegister(registerLabel, registerClass, section) {
  let list = registerTemplate;
  list = list.replace(/\{\$register_label\}/gi, registerLabel);
  list = list.replace(/\{\$register_class\}/gi, registerClass);
  section.append(list);
}

function initRegisters() {
  const section = $('#register');
  registers.forEach((register) => {
    appendRegister(`%${register}`, register, section);
  });
}

function initConditionCodes() {
  const section = $('#condition_code');
  conditionCodes.forEach((register) => {
    appendRegister(register, register, section);
  });
}

function initPipelineRegisters() {
  const section = $('#pipeline_stage');
  pipelineRegisters.forEach((stage) => {
    let list = pipelineTemplate;
    list = list.replace(/\{\$stage_title\}/gi, stage.title);
    list = list.replace(/\{\$stage_id\}/gi, stage.id);
    section.append(list);
    const stageSection = $(`#${stage.id}`);
    stage.registers.forEach((register) => {
      appendRegister(register, register, stageSection);
    });
  });
}

function initAll() {
  initRegisters();
  initConditionCodes();
  initPipelineRegisters();
  values = $('.value');
  icode = $('#writeback').find('.I_CODE');
}

$(() => {
  initAll();
  resetAll();
});

/* Controllers */

let clockTime = 0;
let instructionCount = 0;
let history = [0];  // the history of instruction count

function resetAll() {
  clockTime = 0;
  clock.val(0);
  instructionCount = 0;
  instruction.val(0);
  history = [0];
  cpi.val('N/A');
  values.val(base.attr('data-switch') == 0 ? 0 : '0x0');
}

function setButtons() {
  clockTime === 0 ? previous.attr('disabled', '') : previous.removeAttr('disabled');
  clockTime === window.end ? next.attr('disabled', '') : next.removeAttr('disabled');
}

function disableButtons() {
  restart.attr('disabled', '');
  previous.attr('disabled', '');
  next.attr('disabled', '');
}

function enableButtons() {
  controllers.removeAttr('disabled');
  setButtons();
}

base.on('click', () => {
  const baseText = base.children();
  if (base.attr('data-switch') == 0) {
    // Switches to hexadecimal mode
    values.each((_, element) => {
      const value = $(element);
      const hexValue = parseInt(value.val(), 10).toString(16);
      value.val(`0x${hexValue}`);
    })
    baseText.text('DEC');
    base.attr({
      'mdui-tooltip': `{content: 'Switch to DEC', delay: '1000'}`,
      'data-switch': 1
    });
  } else {
    // Switches to decimal mode
    values.each((_, element) => {
      const value = $(element);
      const decValue = parseInt(value.val(), 16);
      value.val(decValue);
    })
    baseText.text('HEX');
    base.attr({
      'mdui-tooltip': `{content: 'Switch to HEX', delay: '1000'}`,
      'data-switch': 0
    });
  }
});

displayMode.on('click', () => {
  const displayModeIcon = displayMode.children();
  body.toggleClass('mdui-theme-layout-dark');
  if (displayMode.attr('data-switch') == 0) {
    // Switches to night mode
    displayModeIcon.text('brightness_2');
    displayMode.attr({
      'mdui-tooltip': `{content: 'Night mode', delay: '1000'}`,
      'data-switch': 1
    });
  } else {
    // Switches to day mode
    displayModeIcon.text('brightness_7');
    displayMode.attr({
      'mdui-tooltip': `{content: 'Day mode', delay: '1000'}`,
      'data-switch': 0
    });
  }
});

restart.on('click', () => {
  if (restart.attr('disabled') === '') return;
  resetAll();
  setProgressBar();
  setButtons();
});

function getSleepTime() {
  return 10000 / speed.val();
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function setClock(clockTime) {
  clock.val(clockTime);
}

// mode: 0 for previous step, 1 for next step
function setInstructionCountAndCpi(clockTime, mode) {
  const lastInstruction = icode.val();
  // true if the last instruction is I_HALT or I_NOP
  // using '==' instead of '===' to implicitly convert the value to a decimal number
  const isHalt = lastInstruction == 0 || lastInstruction == 1;
  if (mode) {
    instruction.val(instructionCount);
    setCpi(clockTime, instructionCount);
    history.push(instructionCount);
    instructionCount += !isHalt;  // predicts next instruction count
  } else {
    instructionCount = history.pop();
    const curInstructionCount = history[history.length - 1];  // current instruction count
    instruction.val(curInstructionCount);
    setCpi(clockTime, curInstructionCount);
  }
}

function setCpi(clockTime, instructionCount) {
  cpi.val(instructionCount ? clockTime / instructionCount : 'N/A');
}

function setProgressBar() {
  progress.css('width', `${clockTime / window.end * 100}%`);
}

function previousStep() {
  --clockTime;
  outputResult(clockTime, 0);
  setClock(clockTime);
  setInstructionCountAndCpi(clockTime, 0);
  setProgressBar();
}

previous.on('click', () => {
  if (previous.attr('disabled') === '') return;
  previousStep();
  setButtons();
});

function nextStep() {
  outputResult(clockTime, 1);
  ++clockTime;
  setClock(clockTime);
  setInstructionCountAndCpi(clockTime, 1);
  setProgressBar();
}

next.on('click', () => {
  if (next.attr('disabled') === '') return;
  nextStep();
  setButtons();
});

run.on('click', () => {
  if (run.attr('disabled') === '') return;
  const runIcon = run.children();
  if (run.attr('data-switch') == 0) {
    // Run
    run.attr({
      'mdui-tooltip': `{content: 'Pause', delay: '1000'}`,
      'data-switch': 1
    });
    runIcon.text('pause');
    disableButtons();
    (async () => {
      while (clockTime < window.end) {
        if (run.attr('data-switch') == 0) break; // Pause
        nextStep();
        await sleep(getSleepTime());
      }
      run.attr('data-switch', 0);
      runIcon.text('play_arrow');
      enableButtons();
    })();
  } else {
    // Pause
    run.attr({
      'mdui-tooltip': `{content: 'Run', delay: '1000'}`,
      'data-switch': 0
    });
  }
});
