const childProcess = require('child_process');
const fs = require('fs');
const path = require('path');

const express = require('express');
const bodyParser = require('body-parser');
const compression = require('compression');
const formidable = require('formidable');
const favicon = require('serve-favicon');
const temp = require('temp');

const app = express();
const distPath = path.join(__dirname, 'dist');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use('/assets', express.static(distPath));
app.use(favicon(path.join(distPath, 'favicon.ico')));
app.use(compression());

app.get('/', (req, res) => {
  res.sendFile(path.join(distPath, 'index.html'));
});

app.post('/upload', (req, res, next) => {
  let form = new formidable.IncomingForm();
  form.parse(req, (err) => {
    if (err) next(err);
  });
  form.on('fileBegin', (_, file) => {
    fs.mkdir('uploads', (err) => {
      // If the folder already exists, just ignore the error.
      if (err && err.code !== 'EEXIST') next(err);
    })
    file.path = path.join(__dirname, 'uploads', file.name);
  });
  form.on('file', (_, file) => {
    const readStream = fs.createReadStream(file.path);
    const chunks = [];
    readStream.once('error', next);
    readStream.on('data', (chunk) => {
      chunks.push(chunk);
    });
    readStream.once('end', () => {
      const data = Buffer.concat(chunks).toString();
      res.status(200).json({
        fileName: file.name,
        fileData: data
      });
    });
  });
});

app.post('/execute', (req, res, next) => {
  const execFile = childProcess.execFile;
  const program = '../lib/sim';
  let fileName = req.body.fileName;
  let oldInputPath = path.join(__dirname, 'uploads', fileName);
  temp.mkdir('y86-64_sim', (err, tempPath) => {
    let inputPath = path.join(tempPath, 'input.yo');
    let outputDir = tempPath;
    let outputPath = path.join(tempPath, 'changes.json');
    fs.copyFile(oldInputPath, inputPath, (err) => {
      if (err) next(err);
      fs.unlink(oldInputPath, (err) => {
        if (err) next(err);
        // console.log('Removed ' + oldInputPath);
      });
      let sim = execFile(program, [inputPath, outputDir], (err) => {
        if (err) next(err);
        fs.readFile(outputPath, (err, data) => {
          if (err) next(err);
          res.setHeader('Content-Type', 'application/json');
          res.end(data.toString());
          fs.unlink(inputPath, (err) => {
            if (err) next(err);
            // console.log('Removed ' + inputPath);
          });
          fs.unlink(outputPath, (err) => {
            if (err) next(err);
            // console.log('Removed ' + outputPath);
          });
        });
      });
    });
  });
});

app.listen(7273);
