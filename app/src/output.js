'use strict';

const $ = mdui.JQ;

/* Selectors */

const displayMode = $('#display_mode');
const base = $('#base');

/* Output */

// mode: 0 for previous step, 1 for next step
function outputResult(clock, mode) {
  const result = window.result;
  for (let section in result) {
    const id = $(`#${section}`);
    const updates = result[section][clock];
    for (let regid in updates) {
      const register = id.find(`.${regid}`);
      const value = updates[regid][mode ? 'new' : 'old'];
      register.val(base.attr('data-switch') == 0 ? value : `0x${value.toString(16)}`);
      // Animation
      const block = register.closest('.mdui-col');
      block.css('background-color', displayMode.attr('data-switch') == 0 ? '#88a' : '#bbf');
      setTimeout(() => {
        block.css('background-color', 'initial');
      }, 200);
    }
  }
}

export { outputResult };
