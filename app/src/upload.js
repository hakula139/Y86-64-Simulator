'use strict';

let $$ = mdui.JQ;

$$('#uploader').on('click', (error) => {
    let formData = new FormData($$('#uploadForm')[0]);
    $$.ajax({
        method: 'POST',
        url: 'upload',
        data: formData,
        contentType: false,
        processData: false,
        success: (data) => {
            console.log(data);
        },
        error: () => {
            console.log('Cannot connect to server.');
        }
    });
});