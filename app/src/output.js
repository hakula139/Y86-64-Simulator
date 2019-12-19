'use strict';

let $$ = mdui.JQ;

let data = $$('#result');
export let outputResult = (clock) => {
    let result = JSON.parse(data.val());
    console.log(result);
}