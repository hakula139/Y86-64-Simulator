'use strict';

const $ = mdui.JQ;

/* Selectors */

const upload = $('#upload');
const uploadForm = $('#upload_form');
const fileSelect = $('input[name="upload"]');
const objectCode = $('#object-code');
const restart = $('#restart');
const controllers = $('.controller');

/* Controllers */

upload.on('click', () => {
  fileSelect.trigger('click');
});

fileSelect.on('change', () => {
  // Check if fileName is valid
  const fileName = fileSelect.val();
  if (!fileName.length) return;
  if (fileName.substring(fileName.length - 3) !== '.yo') {
    mdui.snackbar({
      message: 'Currently only .yo files are accepted.'
    });
    return;
  }
  uploadFile();
});

// AJAX upload
function uploadFile() {
  const formData = new FormData(uploadForm[0]);
  $.ajax({
    method: 'POST',
    url: 'upload',
    data: formData,
    contentType: false,
    processData: false,
    success: (data) => {
      const dataJson = JSON.parse(data);
      const fileName = dataJson.fileName;
      const fileData = dataJson.fileData;
      console.log(fileName + ' uploaded.');
      objectCode.find('pre').text(fileData);
      execute(fileName);
    }
  });
}

// Receives the result from server side
function execute(fileName) {
  $.ajax({
    method: 'POST',
    url: 'execute',
    data: JSON.stringify({ 'fileName': fileName }),
    contentType: 'application/json; charset=utf-8',
    processData: true,
    success: (data) => {
      window.result = JSON.parse(data);
      window.end = window.result.end;
      controllers.removeAttr('disabled');
      restart.removeAttr('disabled');
      restart.trigger('click');
      mdui.updateSliders();
      mdui.snackbar({
        message: 'Successfully uploaded.'
      });
    }
  });
}

$(document).ajaxError(() => {
  console.log('Failed to upload.');
  controllers.attr('disabled', '');
  mdui.updateSliders();
  mdui.snackbar({
    message: 'Cannot connect to server.'
  });
});
