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
  uploadFile();
});

// AJAX upload
let uploadFile = () => {
  let formData = new FormData($$('#uploadForm')[0]);
  let fileName;
  let fileData;
  $$.ajax({
    method: 'POST',
    url: 'upload',
    data: formData,
    contentType: false,
    processData: false,
    success: (data, textStatus, xhr) => {
      let dataJson = JSON.parse(data);
      fileName = dataJson.fileName;
      fileData = dataJson.fileData;
      console.log('Uploaded ' + fileName);
      $$('#object-code').find('pre').text(fileData);
      operateFile(fileName);
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
        mdui.updateSliders();
        mdui.snackbar({
          message: 'Successfully uploaded.'
        });
      } else {
        $$('.controller').attr('disabled', '');
        mdui.updateSliders();
        mdui.snackbar({
          message: 'Fail to upload.'
        });
      }
    }
  });
}

// Operates the uploaded file
let operateFile = (fileName) => {
  $$.ajax({
    method: 'POST',
    url: 'execute',
    data: JSON.stringify({ 'fileName': fileName }),
    contentType: 'application/json; charset=utf-8',
    processData: true,
    success: (data) => {
      $$('#result').val(data);
      $$('#restart').trigger('click');
    },
    error: (error) => {
      mdui.snackbar({
        message: 'Cannot connect to server.'
      });
    }
  });
}
