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
let registerValues;

/* Templates */

const registerTemplate = $('#register_template').html();
const pipelineTemplate = $('#pipeline_template').html();

/* Initialization */

function appendRegister(register, section) {
  let list = registerTemplate;
  list = list.replace('{$register_label}', register);
  section.append(list);
}

function initRegisters() {
  const section = $('#register');
  registers.forEach((register) => {
    appendRegister(`%${register}`, section);
  });
}

function initConditionCodes() {
  const section = $('#condition_code');
  conditionCodes.forEach((register) => {
    appendRegister(register, section);
  });
}

function initPipelineRegisters() {
  const section = $('#pipeline_stage');
  pipelineRegisters.forEach((stage) => {
    let list = pipelineTemplate;
    list = list.replace('{$stage_title}', stage.title);
    list = list.replace('{$stage_id}', stage.id);
    section.append(list);
    const stageSection = $(`#${stage.id}`);
    stage.registers.forEach((register) => {
      appendRegister(register, stageSection);
    });
  });
}

function initAll() {
  initRegisters();
  initConditionCodes();
  initPipelineRegisters();
  registerValues = $('.value');
}

$(() => {
  initAll();
  resetAll();
});

/* Controllers */

let clock = 0;

function resetAll() {
  registerValues.val(base.attr('data-switch') == 0 ? 0 : '0x0');
  clock = 0;
}

function setButtons() {
  clock === 0 ? previous.attr('disabled', '') : previous.removeAttr('disabled');
  clock === window.end ? next.attr('disabled', '') : next.removeAttr('disabled');
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
    registerValues.each((_, element) => {
      const registerValue = $(element);
      registerValue.val('0x' + parseInt(registerValue.val(), 10).toString(16));
    })
    baseText.text('DEC');
    base.attr('data-switch', 1);
  } else {
    // Switches to decimal mode
    registerValues.each((_, element) => {
      const registerValue = $(element);
      registerValue.val(parseInt(registerValue.val(), 16));
    })
    baseText.text('HEX');
    base.attr('data-switch', 0);
  }
});

displayMode.on('click', () => {
  const displayModeIcon = displayMode.children();
  body.toggleClass('mdui-theme-layout-dark');
  if (displayMode.attr('data-switch') == 0) {
    // Switches to night mode
    displayModeIcon.text('brightness_2');
    displayMode.attr({
      'mdui-tooltip': `{content: 'Night mode'}`,
      'data-switch': 1
    });
  } else {
    // Switches to day mode
    displayModeIcon.text('brightness_7');
    displayMode.attr({
      'mdui-tooltip': `{content: 'Day mode'}`,
      'data-switch': 0
    });
  }
});

restart.on('click', () => {
  if (restart.attr('disabled') === '') return;
  resetAll();
  setButtons();
});

function getSleepTime() {
  return 10000 / speed.val();
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function previousStep() {
  --clock;
  outputResult(clock, 0);
  progress.width(clock / window.end);
}

previous.on('click', () => {
  if (previous.attr('disabled') === '') return;
  previousStep();
  setButtons();
});

function nextStep() {
  outputResult(clock, 1);
  ++clock;
  progress.width(clock / window.end);
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
      'mdui-tooltip': `{content: 'Pause'}`,
      'data-switch': 1
    });
    runIcon.text('pause');
    disableButtons();
    (async () => {
      while (clock < window.end) {
        if (run.attr('data-switch') == 0) break; // Pause
        nextStep();
        await sleep(getSleepTime());
      }
      runIcon.text('play_arrow');
      enableButtons();
    })();
  } else {
    // Pause
    run.attr({
      'mdui-tooltip': `{content: 'Run'}`,
      'data-switch': 0
    });
  }
});
