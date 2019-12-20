'use strict';

import './style.css'
import '../favicon/favicon.ico';

import { outputResult } from './output';

let $$ = mdui.JQ;

// Initialize

let data = $$('#result');
let clock = 0;

// Pipeline registers
let fetch = [
    { 'id': '_PRED_PC', 'label': 'PRED_PC' },
    { 'id': '_CLOCK_CYCLE', 'label': 'CLOCK_CYCLE' }
];
let decode = [
    { 'id': '_STAT', 'label': 'STAT' },
    { 'id': '_I_CODE', 'label': 'I_CODE' },
    { 'id': '_I_FUN', 'label': 'I_FUN' },
    { 'id': '_R_A', 'label': 'R_A' },
    { 'id': '_R_B', 'label': 'R_B' },
    { 'id': '_VAL_C', 'label': 'VAL_C' },
    { 'id': '_VAL_P', 'label': 'VAL_P' }
];
let execute = [
    { 'id': '_STAT', 'label': 'STAT' },
    { 'id': '_I_CODE', 'label': 'I_CODE' },
    { 'id': '_I_FUN', 'label': 'I_FUN' },
    { 'id': '_VAL_C', 'label': 'VAL_C' },
    { 'id': '_VAL_A', 'label': 'VAL_A' },
    { 'id': '_VAL_B', 'label': 'VAL_B' },
    { 'id': '_DST_E', 'label': 'DST_E' },
    { 'id': '_DST_M', 'label': 'DST_M' },
    { 'id': '_SRC_A', 'label': 'SRC_A' },
    { 'id': '_SRC_B', 'label': 'SRC_B' }
];
let memory = [
    { 'id': '_STAT', 'label': 'STAT' },
    { 'id': '_I_CODE', 'label': 'I_CODE' },
    { 'id': '_CND', 'label': 'CND' },
    { 'id': '_VAL_E', 'label': 'VAL_E' },
    { 'id': '_VAL_A', 'label': 'VAL_A' },
    { 'id': '_DST_E', 'label': 'DST_E' },
    { 'id': '_DST_M', 'label': 'DST_M' }
];
let writeback = [
    { 'id': '_STAT', 'label': 'STAT' },
    { 'id': '_I_CODE', 'label': 'I_CODE' },
    { 'id': '_VAL_E', 'label': 'VAL_E' },
    { 'id': '_VAL_M', 'label': 'VAL_M' },
    { 'id': '_DST_E', 'label': 'DST_E' },
    { 'id': '_DST_M', 'label': 'DST_M' }
];
let pipelineRegister = [
    { 'stage': fetch, 'id': '#fetch' },
    { 'stage': decode, 'id': '#decode' },
    { 'stage': execute, 'id': '#execute' },
    { 'stage': memory, 'id': '#memory' },
    { 'stage': writeback, 'id': '#writeback' }
];

// Registers
let processorRegister = [
    { 'id': '_RAX', 'label': '%RAX' },
    { 'id': '_RCX', 'label': '%RCX' },
    { 'id': '_RDX', 'label': '%RDX' },
    { 'id': '_RBX', 'label': '%RBX' },
    { 'id': '_RSP', 'label': '%RSP' },
    { 'id': '_RBP', 'label': '%RBP' },
    { 'id': '_RSI', 'label': '%RSI' },
    { 'id': '_RDI', 'label': '%RDI' },
    { 'id': '_R8', 'label': '%R8' },
    { 'id': '_R9', 'label': '%R9' },
    { 'id': '_R10', 'label': '%R10' },
    { 'id': '_R11', 'label': '%R11' },
    { 'id': '_R12', 'label': '%R12' },
    { 'id': '_R13', 'label': '%R13' },
    { 'id': '_R14', 'label': '%R14' }
];

// Condition code registers
let conditionCode = [
    { 'id': '_OF', 'label': 'OF' },
    { 'id': '_SF', 'label': 'SF' },
    { 'id': '_ZF', 'label': 'ZF' }
];

// Set buttons
let previous = $$('#previous');
let next = $$('#next');
let setButton = () => {
    let result = data.val();
    let end = result['end']['end'];
    if (clock == 0)
        previous.attr('disabled', '');
    else
        previous.removeAttr('disabled');
    if (clock > end)
        next.attr('disabled', '');
    else
        next.removeAttr('disabled');
}

let clearAll = () => {
    $$('.mdui-textfield-input').val(0);
    clock = 0;
}

let template = $$('#list-template').html();
let replaceLabels = (register, id) => {
    let list = template;
    list = list.replace(/\{\$registerId\}/gi, register.id);
    list = list.replace(/\{\$registerLabel\}/gi, register.label);
    $$(id).append(list);
};
(() => {
    pipelineRegister.forEach((stageItem) => {
        let stage = stageItem.stage;
        let stageId = stageItem.id;
        stage.forEach((register) => {
            replaceLabels(register, stageId);
        });
    });
    processorRegister.forEach((register) => {
        replaceLabels(register, '#register');
    });
    conditionCode.forEach((register) => {
        replaceLabels(register, '#condition_code');
    });
    clearAll();
})();

// Monitor the buttons

// Display mode
let displayMode = $$('#display-mode');
let displayModeIcon = $$('#display-mode-icon');
displayMode.on('click', (error) => {
    $$('body').toggleClass('mdui-theme-layout-dark');
    displayMode.attr('mdui-tooltip',
        displayMode.attr('mdui-tooltip') === '{content: \'Day mode\'}' ?
            '{content: \'Night mode\'}' : '{content: \'Day mode\'}'
    );
    displayModeIcon.html(
        displayModeIcon.html() === 'brightness_7' ? 'brightness_2' : 'brightness_7'
    );
})

// Restart
let restart = $$('#restart');
restart.on('click', (error) => {
    if (restart.attr('disabled') === '') return;
    clearAll();
    setButton();
})

// Speed controller
let speedController = $$('#speed-controller-slider');
let getSleepTime = () => {
    let speed = speedController.val();
    let sleepTime = 10000 / speed;
    return sleepTime;
}
let sleep = (ms) => {
    return new Promise((resolve) => setTimeout(resolve, ms));
}

// Previous
let previousStep = () => {
    --clock;
    outputResult(clock, 0);
    $$('#_CLOCK_CYCLE').find('input').val(clock);
}
previous.on('click', (error) => {
    if (previous.attr('disabled') === '') return;
    previousStep();
    setButton();
})

// Next
let nextStep = () => {
    outputResult(clock, 1);
    ++clock;
    $$('#_CLOCK_CYCLE').find('input').val(clock);
}
next.on('click', (error) => {
    if (next.attr('disabled') === '') return;
    nextStep();
    setButton();
})

// Run status
let runStatus = $$('#run-status');
let runStatusIcon = $$('#run-status-icon');
runStatus.on('click', (error) => {
    if (runStatus.attr('disabled') === '') return;
    let result = data.val();
    let end = result['end']['end'];
    if (runStatusIcon.html() === 'play_arrow') {
        runStatus.attr('mdui-tooltip', '{content: \'Pause\'}');
        runStatusIcon.html('pause');
        restart.attr('disabled', '');
        previous.attr('disabled', '');
        next.attr('disabled', '');
        (async () => {
            while (clock <= end) {
                if (runStatusIcon.html() === 'play_arrow') break;
                await sleep(getSleepTime());
                nextStep();
            }
            runStatusIcon.html('play_arrow');
            restart.removeAttr('disabled');
            setButton();
        })();
    } else {
        runStatus.attr('mdui-tooltip', '{content: \'Run\'}');
        runStatusIcon.html('play_arrow');
    }
})