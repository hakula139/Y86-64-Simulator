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
        mdui.snackbar({
            message: 'Currently only .yo files are accepted.'
        });
        return;
    }

    // AJAX upload
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
        error: () => {
            mdui.snackbar({
                message: 'Cannot connect to server.'
            });
        },
        complete: (xhr, textStatus) => {
            if (textStatus === 'success') {
                $$('.controller').removeAttr('disabled');
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

    // Operates the uploaded file
    let operateFile = (filename) => {
        $$.ajax({
            method: 'POST',
            url: 'execute',
            data: JSON.stringify({ 'filename': filename }),
            contentType: 'application/json; charset=utf-8',
            processData: true,
            success: (data) => {
                console.log(data);
            },
            error: () => {
                mdui.snackbar({
                    message: 'Cannot connect to server.'
                });
            }
        });
    };
});
