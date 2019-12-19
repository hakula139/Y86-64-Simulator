'use strict';

let $$ = mdui.JQ;

let data = $$('#result');
let previous = $$('#previous');
let next = $$('#next');

export let outputResult = (clock, mode) => {
    let result = data.val();
    let end = result['end']['end'];
    for (let key in result) {
        let id = $$('#' + key);
        let section = result[key][clock];
        for (let register in section) {
            let regid = $$('#_' + register).find('input');
            let runMode = mode ? 'new' : 'old';
            regid.val(section[register][runMode]);
        }
    }
}