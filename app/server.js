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
const uploadPath = path.join(__dirname, 'uploads');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use('/assets', express.static(distPath));
app.use(favicon(path.join(distPath, 'favicon.ico')));
app.use(compression());  // using gzip

app.get('/', (req, res) => {
  res.sendFile(path.join(distPath, 'index.html'));
});

app.post('/upload', (req, res, next) => {
  let form = new formidable.IncomingForm();
  form.parse(req, (err) => {
    if (err) next(err);
  });
  form.on('fileBegin', (_, file) => {
    fs.mkdir(uploadPath, { recursive: true }, (err) => {
      if (err) next(err);
      // console.log('Created ' + uploadPath);
    })
    file.path = path.join(uploadPath, file.name);
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
      res.json({
        fileName: file.name,
        fileData: data
      });
    });
  });
});

app.post('/execute', (req, res, next) => {
  const execFile = childProcess.execFile;
  const program = '../lib/sim';
  const fileName = req.body.fileName;
  const oldInputPath = path.join(uploadPath, fileName);
  temp.mkdir('y86-64_sim_', (err, tempPath) => {
    if (err) next(err);
    // console.log('Created ' + tempPath);
    const inputPath = path.join(tempPath, 'input.yo');
    const outputPath = path.join(tempPath, 'changes.json');
    // Tries moving if possible, otherwise falls back to copying.
    fs.rename(oldInputPath, inputPath, (err) => {
      if (err) {
        if (err.code !== 'EXDEV') next(err);
        // Failed to move
        const readStream = fs.createReadStream(oldInputPath);
        const writeStream = fs.createWriteStream(inputPath);
        readStream.pipe(writeStream);
        readStream.once('error', next);
        readStream.once('end', () => {
          fs.unlink(oldInputPath, (err) => {
            if (err) next(err);
            // console.log('Removed ' + oldInputPath);
          });
        });
      }
      // Usage: sim input_file output_dir
      execFile(program, [inputPath, tempPath], (err) => {
        if (err) next(err);
        const readStream = fs.createReadStream(outputPath);
        res.setHeader('Content-Type', 'application/json');
        readStream.pipe(res);
        readStream.once('error', next);
        readStream.once('end', () => {
          fs.rmdir(tempPath, { recursive: true }, (err) => {
            if (err) next(err);
            // console.log('Removed ' + tempPath);
          });
        });
      });
    });
  });
});

app.listen(7273);
