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
            if (data[code] === 200) {
                $$('#status').html('Success');
                console.log('Upload success: ', data[filename]);
            } else {
                $$('#status').html('Failed');
                console.log('Upload failed.');
            }
        },
        error: () => {
            $$('#status').html('Disconnected');
            console.log('Cannot connect to server.');
        }
    });
});