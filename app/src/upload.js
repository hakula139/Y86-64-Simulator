'use strict';

let $$ = mdui.JQ;

let uploader = $$('#uploader');
let fileSelect = $$('#fileSelect');

uploader.on('click', (error) => {
    fileSelect.trigger('click');
});

fileSelect.on('change', (error) => {
    // Check filename
    const filename = fileSelect.val();
    const filenameLength = filename.length;
    if (!filenameLength) return;
    if (filename.substring(filenameLength - 3) != '.yo') {
        mdui.snackbar({
            message: 'Currently only .yo files are accepted.'
        });
        return;
    }
    uploadFile();
});

// AJAX upload
let uploadFile = () => {
    let formData = new FormData($$('#uploadForm')[0]);
    let filename;
    $$.ajax({
        method: 'POST',
        url: 'upload',
        data: formData,
        contentType: false,
        processData: false,
        success: (data, textStatus, xhr) => {
            filename = JSON.parse(data)['file'];
            console.log('Uploaded ' + filename);
            operateFile(filename);
        },
        error: (error) => {
            mdui.snackbar({
                message: 'Cannot connect to server.'
            });
        },
        complete: (xhr, textStatus) => {
            if (textStatus === 'success') {
                $$('.controller').removeAttr('disabled');
                $$('#previous').attr('disabled', '');
                mdui.snackbar({
                    message: 'Successfully uploaded.'
                });
            } else {
                $$('.controller').attr('disabled', '');
                mdui.snackbar({
                    message: 'Fail to upload.'
                });
            }
        }
    });
}

// Operates the uploaded file
let operateFile = (filename) => {
    $$.ajax({
        method: 'POST',
        url: 'execute',
        data: JSON.stringify({ 'filename': filename }),
        contentType: 'application/json; charset=utf-8',
        processData: true,
        success: (data) => {
            $$('#result').val(JSON.parse(data));
            $$('#restart').trigger('click');
        },
        error: (error) => {
            mdui.snackbar({
                message: 'Cannot connect to server.'
            });
        }
    });
}
