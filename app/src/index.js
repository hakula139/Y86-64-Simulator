'use strict';

import './style.css'
import '../favicon/favicon.ico';

import { outputResult } from './output';

let $$ = mdui.JQ;

// Initializes pipeline register section

let fetch = [
    { 'id': '_PRED_PC', 'label': 'PRED_PC' }
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

let conditionCode = [
    { 'id': '_OF', 'label': 'OF' },
    { 'id': '_SF', 'label': 'SF' },
    { 'id': '_ZF', 'label': 'ZF' }
];

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
    $$('.mdui-textfield-input').val(0);
})();

// Monitors the buttons

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

// Run status
let runStatus = $$('#run-status');
let runStatusIcon = $$('#run-status-icon');
runStatus.on('click', (error) => {
    if (runStatus.attr('disabled') === '') return;
    runStatus.attr('mdui-tooltip',
        runStatus.attr('mdui-tooltip') === '{content: \'Run\'}' ?
            '{content: \'Pause\'}' : '{content: \'Run\'}'
    );
    runStatusIcon.html(
        runStatusIcon.html() === 'play_arrow' ? 'pause' : 'play_arrow'
    );
})

let clock = 0;

// Previous
let previous = $$('#previous');
let previousIcon = $$('#previous-icon');
previous.on('click', (error) => {
    if (previous.attr('disabled') === '') return;
    --clock;
})

// Next
let next = $$('#next');
let nextIcon = $$('#next-icon');
next.on('click', (error) => {
    if (next.attr('disabled') === '') return;
    ++clock;
    outputResult(clock);
})
