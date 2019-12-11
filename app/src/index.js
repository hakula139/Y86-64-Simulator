import './style.css'
let $$ = mdui.JQ;

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
let pipeline = [
    { 'stage': fetch, 'id': '#fetch' },
    { 'stage': decode, 'id': '#decode' },
    { 'stage': execute, 'id': '#execute' },
    { 'stage': memory, 'id': '#memory' },
    { 'stage': writeback, 'id': '#writeback' }
];

let template = $$('#list-template').html();
(function () {
    let list;
    pipeline.forEach(function (stageItem) {
        let stage = stageItem.stage;
        let stageId = stageItem.id;
        stage.forEach(function (register) {
            list = template;
            list = list.replace(/\{\$registerId\}/gi, register.id);
            list = list.replace(/\{\$registerLabel\}/gi, register.label);
            $$(stageId).append(list);
        });
    });
})();
