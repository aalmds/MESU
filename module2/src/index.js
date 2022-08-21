function requestPayload() {
  return {
    url: 'https://connector.koretmdata.com.br/api/v2/payloadHistories',
    requestData: {
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
      method: 'POST',
      headers: {
        'Accept': 'application/json',
        'Authorization': 'GpSc1jWN8QFCZRL2KVbP4OTlKEcRvQJ',
        'Content-Type': 'application/json; charset=UTF-8'
      }
    }
  }
}

async function requestKora() {
  const payload = requestPayload()
  try {
    const response = await fetch(payload.url, {
      method: "POST",
      body: JSON.stringify(payload.requestData),
      headers: payload.options.headers
    })
    const data = response.json()
    return await data
  } catch (err) {
    return console.info(err)
  }
}

function base64ToHex(str) {
  const raw = atob(str);
  let result = '';
  for (let i = 0; i < raw.length; i++) {
    const hex = raw.charCodeAt(i).toString(16);
    result += (hex.length === 2 ? hex : '0' + hex);
  }
  return result.toUpperCase();
}

function parsePayloadResponse(data) {
  console.info(data)
  p = JSON.parse(data.payload).params.payload
  console.info(p)
  converted = base64ToHex(p).toString('hex')
  console.info(converted)
  var obj = {
    'level': converted[0],
    'sensor1_status': converted[1],
    'sensor2_status': converted[2],
    'sensor3_status': converted[3]
  }

  return obj
}
function setDate(date) {
  var d = new Date(date);
  var h = d.getHours();
  d.setHours(h - 3);  // mudando o fuso horário
  return d;
}

async function getLastData() {
  const data = await requestKora()
  console.info(data)
  return data[data.length - 1]
}

async function mapToTable() {
  const response = await requestKora()
  let rowIndex = 0
  const tableData = response.map((obj) => {
    return {
      data: parsePayloadResponse(obj),
      date: obj.created_at
    }
  }).reverse().slice(0, 5)
  for (let i = 1; i <= tableData.length; i++) {
    let htmlElement = document.getElementById('previousData').children[0].children[i].children
    htmlElement[0].innerText = setDate(new Date(tableData[i - 1].date)).toUTCString()
    htmlElement[1].innerText = translateLevel(tableData[i - 1].data.level)

  }
}

function translateLevel(level) {
  const levelMap = {
    '0': 'Muito Baixo - abaixo de 5 cm',

    '1': 'Baixo - cerca de 5 cm',

    '2': 'Médio - cerca de 15 cm',

    '3': 'Alto - cerca de 38 cm',
    '4': 'Muito alto -  cerca de 48 cm',
    '5': 'Erro'
  }
  return levelMap[level]
}


async function updateWaterLevel() {
  const levelElement = document.getElementById("level")
  let lastData = parsePayloadResponse(await getLastData())
  let level = translateLevel(lastData.level)
  levelElement.innerText = level 
  let statusElement = document.getElementById('status')
  if(level == 'Erro') {
    statusElement.innerText = 'Erro no '
    if(lastData.sensor1_status == '0') {
      statusElement.innerText += 'sensor 1'
    } 
    if(lastData.sensor2_status == '0') {
      statusElement.innerText += 'sensor 2'
    }
    if(lastData.sensor3_status == '0') {
      statusElement.innerText += 'sensor 3'
    }
  }else {
    statusElement.innerText = 'Ok'
  }
}

async function main() {
  setInterval(
    async () => {
      await updateWaterLevel()
      await mapToTable()
    },
    3000
  )

}

function showTable(){
  let element =   document.getElementById("previousData")
  if(element.classList[0] == 'hide'){
    element.classList.remove('hide')
    element.classList.add('show')
  } else {
    element.classList.remove('show')
    element.classList.add('hide')  
  } 
}


document.addEventListener("DOMContentLoaded", main);