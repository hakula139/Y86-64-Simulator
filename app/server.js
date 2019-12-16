'use strict';

const express = require('express');
const formidable = require('formidable');

let app = express();
let distPath = __dirname + '/dist/';
app.use(express.static(distPath));

app.get('/', (request, response) => {
    response.sendFile(distPath + '/index.html');
});

app.post('/upload', (request, response) => {
    let form = new formidable.IncomingForm();
    let index, filename;
    form.parse(request);
    form.on('field', (name, value) => {
        if (name == 'index') index = value;
    });
    form.on('fileBegin', (name, file) => {
        file.path = __dirname + '/uploads/' + file.name;
    });
    form.on('file', (name, file) => {
        filename = file.name;
    });
    form.on('end', () => {
        response.json({
            code: 200,
            index: index,
            filename: filename
        });
    });
    form.on('error', () => {
        response.end('Something went wrong on the server side. Upload failed.');
    });
});

app.listen(7273);
