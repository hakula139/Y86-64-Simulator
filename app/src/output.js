'use strict';

let $$ = mdui.JQ;

let data = $$('#result');
let baseModeText = $$('#base-mode-text');
export let outputResult = (clock, mode) => {
    let result = data.val();
    for (let key in result) {
        let id = $$('#' + key);
        let section = result[key][clock];
        for (let register in section) {
            let regid = id.find('#_' + register).children('input');
            let runMode = mode ? 'new' : 'old';
            let value = section[register][runMode];
            regid.val(baseModeText.html() === 'HEX' ? value : '0x' + value.toString(16));
        }
    }
}