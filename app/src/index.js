'use strict';

import './style.css'
import '../favicon/favicon.ico';

import { outputResult } from './output';

let $$ = mdui.JQ;

// Initializes pipeline register section

let fetch = [
    { 'id': 'predpc', 'label': 'PRED_PC' }
];
let decode = [
    { 'id': 'stat', 'label': 'STAT' },
    { 'id': 'icode', 'label': 'I_CODE' },
    { 'id': 'ifun', 'label': 'I_FUN' },
    { 'id': 'ra', 'label': 'R_A' },
    { 'id': 'rb', 'label': 'R_B' },
    { 'id': 'valc', 'label': 'VAL_C' },
    { 'id': 'valp', 'label': 'VAL_P' }
];
let execute = [
    { 'id': 'stat', 'label': 'STAT' },
    { 'id': 'icode', 'label': 'I_CODE' },
    { 'id': 'ifun', 'label': 'I_FUN' },
    { 'id': 'valc', 'label': 'VAL_C' },
    { 'id': 'vala', 'label': 'VAL_A' },
    { 'id': 'valb', 'label': 'VAL_B' },
    { 'id': 'dste', 'label': 'DST_E' },
    { 'id': 'dstm', 'label': 'DST_M' },
    { 'id': 'srca', 'label': 'SRC_A' },
    { 'id': 'srcb', 'label': 'SRC_B' }
];
let memory = [
    { 'id': 'stat', 'label': 'STAT' },
    { 'id': 'icode', 'label': 'I_CODE' },
    { 'id': 'cnd', 'label': 'CND' },
    { 'id': 'vale', 'label': 'VAL_E' },
    { 'id': 'vala', 'label': 'VAL_A' },
    { 'id': 'dste', 'label': 'DST_E' },
    { 'id': 'dstm', 'label': 'DST_M' }
];
let writeback = [
    { 'id': 'stat', 'label': 'STAT' },
    { 'id': 'icode', 'label': 'I_CODE' },
    { 'id': 'vale', 'label': 'VAL_E' },
    { 'id': 'valm', 'label': 'VAL_M' },
    { 'id': 'dste', 'label': 'DST_E' },
    { 'id': 'dstm', 'label': 'DST_M' }
];
let pipelineRegister = [
    { 'stage': fetch, 'id': '#fetch' },
    { 'stage': decode, 'id': '#decode' },
    { 'stage': execute, 'id': '#execute' },
    { 'stage': memory, 'id': '#memory' },
    { 'stage': writeback, 'id': '#writeback' }
];

let processorRegister = [
    { 'id': 'rax', 'label': '%RAX' },
    { 'id': 'rcx', 'label': '%RCX' },
    { 'id': 'rdx', 'label': '%RDX' },
    { 'id': 'rbx', 'label': '%RBX' },
    { 'id': 'rsp', 'label': '%RSP' },
    { 'id': 'rbp', 'label': '%RBP' },
    { 'id': 'rsi', 'label': '%RSI' },
    { 'id': 'rdi', 'label': '%RDI' },
    { 'id': 'r8', 'label': '%R8' },
    { 'id': 'r9', 'label': '%R9' },
    { 'id': 'r10', 'label': '%R10' },
    { 'id': 'r11', 'label': '%R11' },
    { 'id': 'r12', 'label': '%R12' },
    { 'id': 'r13', 'label': '%R13' },
    { 'id': 'r14', 'label': '%R14' }
];

let conditionCode = [
    { 'id': 'of', 'label': 'OF' },
    { 'id': 'sf', 'label': 'SF' },
    { 'id': 'zf', 'label': 'ZF' }
];

export let clock = 0;

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
        replaceLabels(register, '#condition-code');
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
