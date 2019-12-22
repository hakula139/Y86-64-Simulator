'use strict';

let $$ = mdui.JQ;

let data = $$('#result');
export let outputResult = (clock, mode) => {
    let result = data.val();
    for (let key in result) {
        let id = $$('#' + key);
        let section = result[key][clock];
        for (let register in section) {
            let regid = id.find('#_' + register).children('input');
            let runMode = mode ? 'new' : 'old';
            regid.val(section[register][runMode]);
        }
    }
}