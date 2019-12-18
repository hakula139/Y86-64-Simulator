'use strict';

const childProcess = require('child_process');
const fs = require('fs');
const path = require('path');

const express = require('express');
const bodyParser = require('body-parser');
const formidable = require('formidable');
const favicon = require('serve-favicon');
const temp = require('temp');

let app = express();
let distPath = path.join(__dirname, 'dist');
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(distPath));
app.use(favicon(path.join(distPath, 'favicon.ico')));

app.get('/', (request, response, next) => {
    response.sendFile(path.join(distPath, 'index.html'));
});

app.post('/upload', (request, response, next) => {
    let form = new formidable.IncomingForm();
    let filename;
    form.parse(request, (error) => {
        if (error) next(error);
    });
    form.on('fileBegin', (name, file) => {
        file.path = path.join(__dirname, 'uploads', file.name);
    });
    form.on('file', (name, file) => {
        filename = file.name;
    });
    form.on('end', () => {
        response.status(200).json({
            'file': filename
        });
    });
    form.on('error', () => {
        response.status(503).end(
            'Something went wrong on the server side.'
        );
    });
});

app.post('/execute', (request, response, next) => {
    const execFile = childProcess.execFile;
    const program = '../lib/sim';
    let filename = request.body.filename;
    let oldInputPath = path.join(__dirname, 'uploads', filename);
    temp.mkdir('y86-64_sim', (error, tempPath) => {
        let inputPath = path.join(tempPath, 'input.yo');
        let outputDir = tempPath;
        let outputPath = path.join(tempPath, 'changes.json');
        fs.copyFile(oldInputPath, inputPath, (error) => {
            if (error) next(error);
            fs.unlink(oldInputPath, (error) => {
                if (error) next(error);
                // console.log('Removed ' + oldInputPath);
            });
            let sim = execFile(program, [inputPath, outputDir], (error) => {
                if (error) next(error);
                fs.readFile(outputPath, (error, data) => {
                    if (error) next(error);
                    response.setHeader('Content-Type', 'application/json');
                    response.end(data.toString());
                });
            });
        });
    });
});

app.listen(7273);
