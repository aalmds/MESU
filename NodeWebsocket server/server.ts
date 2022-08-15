import express = require('express');
import bodyParser = require("body-parser");
import { SensorData } from './sensorData';
import WebSocketServer = require('websocket');
import * as fs from 'fs';

var database: SensorData[] = [];

var app = express();

var port = 8080;

var allowCrossDomain = function(req: any, res: any, next: any) {
    res.header('Access-Control-Allow-Origin', "*");
    res.header('Access-Control-Allow-Methods', 'GET, PUT, POST, DELETE, OPTIONS');
    res.header('Access-Control-Allow-Headers', 'Content-Type, Authorization, X-Requested-With');
    next();
}
app.use(allowCrossDomain);

app.use(bodyParser.json());


var server = app.listen(port, function () {
  console.log(`Server listening on port ${port}`);
})

var wsServer = new WebSocketServer.server({
    httpServer: server
  });

function closeServer(): void {
  server.close();
}

wsServer.on('request', (request) => {
    //Aceita a conexão do client
    let client = request.accept(null, request.origin);

    //Chamado quando o client envia uma mensagem
    client.on('message', (message) => {

        console.log('Received: \n' + message.utf8Data);

        const msg = <SensorData> JSON.parse(message.utf8Data);

        const newData = new SensorData(msg);
        
        readData();
        if(newData != undefined || newData != {}){
            database.push(newData);
            saveData(database);
            console.log('\nStored: \n' + newData);
        }
    });

    //Chamado quando a conexão com o client é fechada
    client.on('close', () => {
        console.log("Conexão fechada");
    });
});


function readData(){
    fs.readFile("sensorDatabase.json", "utf-8", (err, dt) => {
        if (err) {
          console.log(err);
        }
        else {
          database = JSON.parse(dt);
        }
      });
}

function saveData(data){
    fs.writeFile("sensorDatabase.json", JSON.stringify(data), function(err) {
        if(err) {
            console.log(err);
        } else {
            console.log("The file was saved!");
        }
    })
}

export { app, server, closeServer }