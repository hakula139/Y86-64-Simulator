'use strict';

let $$ = mdui.JQ;

let data = $$('#result');
export let outputResult = (clock) => {
    let result = JSON.parse(data.val());
    for (let key in result) {
        let id = $$('#' + key);
        let section = result[key][clock];
        for (let register in section) {
            let regid = $$('#_' + register).find('input');
            regid.val(section[register]['new']);
        }
    }
}