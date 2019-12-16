'use strict';

let $$ = mdui.JQ;

let uploader = $$('#uploader');
let fileSelect = $$('#fileSelect');

uploader.on('click', (error) => {
    fileSelect.trigger('click');
});

fileSelect.on('change', (error) => {
    // Check filename
    const fileName = fileSelect.val();
    const fileNameLength = fileName.length;
    if (!fileNameLength) return;
    if (fileName.substring(fileNameLength - 3) != '.yo') {
        alert('Currently only .yo files are accepted.');
        return;
    }

    // AJAX upload
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
        },
        complete: (xhr, textStatus) => {
            if (textStatus === 'success') {
                $$('.controller').removeAttr('disabled');
            } else {
                $$('.controller').attr('disabled', '');
            }
        }
    });
});
