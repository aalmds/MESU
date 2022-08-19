let request;  
const setup = () => {
    request = {
        payload: { 
            "devEUIs": [
                "000516800010c962"
            ],
            "period": 48,
            "type": [
                "uplink"
            ],
            "extension": "JSON" 
        }, 
        options: {
            host: payloads,
            path: path,
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Authorization': 'GpSc1jWN8QFCZRL2KVbP4OTlKEcRvQJ',
                'Content-Type': 'application/json; charset=UTF-8'
            }
        }
    }
}

const callKora = () => {

}


/**
function updateLevel() {
    let levelElement = document.getElementById('level')
    let koraData = getKoraData(requestData,options)
    let currentInfo = { 
            "data": koraData[koraData.length - 1]["sensorData"], 
            "date": koraData[koraData.length - 1]["date"],
            "device_id": koraData[koraData.length - 1]["device_id"]
        }

    const level = currentInfo.data.level 
    levelElement.innerText = levelTranslation[level] 
    let statusElement = document.getElementById('status')
    statusElement.innerText = 'OK'
    if(level > 2) {

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
**/

function saveData(data){
    fs.writeFile("level.json", JSON.stringify(data), function(err) {
        if(err) {
            console.log(err);
        } else {
            console.log("The file was saved!");
        }
    })
}

function saveLevel() {
    const koraData = getKoraData(requestData, options) 
    saveData(koraData[koraData.length -1 ])
}

saveLevel() 
//getKoraData(requestData, options)