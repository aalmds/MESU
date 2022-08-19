// let request;  
// const setup = () => {
//     request = {
//         payload: { 
//             "devEUIs": [
//                 "000516800010c962"
//             ],
//             "period": 48,
//             "type": [
//                 "uplink"
//             ],
//             "extension": "JSON" 
//         }, 
//         options: {
//             host: payloads,
//             path: path,
//             method: 'POST',
//             headers: {
//                 'Accept': 'application/json',
//                 'Authorization': 'GpSc1jWN8QFCZRL2KVbP4OTlKEcRvQJ',
//                 'Content-Type': 'application/json; charset=UTF-8'
//             }
//         }
//     }
// }

// const callKora = () => {

// }

async function readFile() {
    return await fetch('./level.json')
        .then((response) => response.json())
        .then((json) => {
            return json
        });
}

function readData(){
    fs.readFile("level.json", "utf-8", (err, d) => {
        if (err) {
            console.log(err);
        }
        else {
            //console.log(data);
            var dado = JSON.parse(d);;
            data = dado;
            console.log("Reading data:");
            console.log(data);
        }
      });
}



async function updateLevel() {
    const levelTranslation = {
        '0' : 'BAIXO',
        '1' : 'MEDIO',
        '2' : 'ALTO',
        '3' : 'GRAVE'
    }

    let levelElement = document.getElementById('level')
    let koraData =  await readFile();
    console.info(koraData)
    let currentInfo = { 
            "data": koraData[koraData.length - 2]["sensorData"], 
            "date": koraData[koraData.length - 2]["date"],
            "device_id": koraData[koraData.length - 2]["device_id"]
        }

    const level = currentInfo.data.level 
    levelElement.innerText = levelTranslation[level] 
    let statusElement = document.getElementById('status')
    statusElement.innerText = 'OK'
    if(level > 3) {

        let id_problem = currentInfo.device_id

        if (currentInfo.data.sensor1_status=="0"){
            statusElement.innerText = 'ERRO NO SENSOR 1\n'
        }if (currentInfo.data.sensor2_status=="0"){
            statusElement.innerText = 'ERRO NO SENSOR 2\n'
        }if (currentInfo.data.sensor3_status=="0"){
            statusElement.innerText = 'ERRO NO SENSOR 3\n'
        }
        alert(`NECESSIDADE DE REPARO TÉCNICO NA TORRE ${id_problem}`)



    }
}

function saveLevel() {
    const koraData = readData(requestData, options) 
    saveData(koraData[koraData.length -1 ])
}
