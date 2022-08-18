var express = require('express');
var app = express();
const https = require('https');

var p = 3000

var payloads = "connector.koretmdata.com.br"
var path = "/api/v2/payloadHistories"

var payloadsHistory = []

const options = {
  host: payloads,
  path: path,
  method: 'POST',
  headers: {
    'Accept': 'application/json',
    'Authorization': 'GpSc1jWN8QFCZRL2KVbP4OTlKEcRvQJ',
    'Content-Type': 'application/json; charset=UTF-8'
  }
};

const requestData = {
    "devEUIs": [
      "000516800010c962"
    ],
    "period": 48,
    "type": [
      "uplink"
    ],
    "extension": "JSON"
};

function setDate(date) {
    const d = new Date(date);
    var h = d.getHours();
    d.setHours(h - 3);  // mudando o fuso horário
    return d;
}

function getSensorsData(payload){
    p = JSON.parse(payload)
    converted = Buffer.from(p.params.payload, 'base64').toString('hex')
    var obj = {
        'level': converted[0],
        'sensor1_status': converted[1],
        'sensor2_status': converted[2],
        'sensor3_status': converted[3]
    }
    return obj
}

function getDeviceName(payload){
    p = JSON.parse(payload)
    return p.meta.device_name
}

function getKoraData(requestData, options) {
    var dataset = []
    const request = https.request(options, (res) => {
        if (res.statusCode !== 200) {
            console.error(`ERROR CODE: ${res.statusCode}`);
            res.resume();
            return;
        }
    
        let data = '';
    
        res.on('data', (chunk) => {
            data += chunk;
        });
    
        res.on('close', () => {
            console.log("entrei!!")
            dataset = JSON.parse(data); 
            console.log(dataset[53]) 
            for(var i=0; i<dataset.length; i++){
                var obj = dataset[i];
                payloadsHistory.push({ 
                    "device_id": obj["device_id"],
                    "devEUI": obj["devEUI"],
                    "sensorData": getSensorsData(obj['payload']),
                    "date": setDate(obj['created_at']),
                    "KORA_device_name": getDeviceName(obj['payload'])
                });
                // params.payload
                // meta.device_name
                // console.log(payloadsHistory[i])
                console.log(payloadsHistory[i])
            }
        });
    });

    request.write(JSON.stringify(requestData));
    
    request.end();
    
    request.on('error', (err) => {
      console.error(`Encountered an error trying to make a request: ${err.message}`);
    });
}

getKoraData(requestData, options)