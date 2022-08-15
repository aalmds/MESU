var WebSocketServer = require('websocket').server;
var http = require('http');
var fs = require('fs');


//Porta que o server irá escutar
const port = 8080;

//Cria o server 
var server = http.createServer();

class Data {
    constructor (id, l, d){
        this.id = id;
        this.level = l;
        this.date = d;
    }
}


var data= [];

//Server irá escutar na porta definida em 'port'
server.listen(port, () => {
    //Server está pronto
    console.log(`Server está executando na porta ${port}`);
});

//Cria o WebSocket server
wsServer = new WebSocketServer({
  httpServer: server
});

//Chamado quando um client deseja conectar
wsServer.on('request', (request) => {
    //Estado do led: false para desligado e true para ligado
    let state = false;

    //Aceita a conexão do client
    let client = request.accept(null, request.origin);

    //Chamado quando o client envia uma mensagem
    client.on('message', (message) => {

        //console.log('Received: \n' + message.utf8Data);
        // var msg = message.utf8Data;
        var temp = message.utf8Data;

        //console.log(temp);
        if(data.length!=0){
            readData(data);
            data.push(temp);
            saveData(data);
        }
        else{
            data.push(temp);
            saveData(data);
        }
        
        // if (data1 != null){
        //     data1.push(temp);
        //     saveData(data1);
        // }
        

        // console.log(message.weight);
        // console.log(message.light);
        // console.log(message.humid);
        //Se é uma mensagem string utf8
        // if (message.type === 'utf8') {
        //     //Mostra no console a mensagem
        //     console.log(message.utf8Data);
        // }
    });

    //Cria uma função que será executada a cada 1 segundo (1000 millis) para enviar o estado do led
    let interval = setInterval(() => {
        //Envia para o client "ON" ou "OFF" dependendo do estado atual da variável state
        client.sendUTF(state? "ON" : "OFF");
        //Inverte o estado
        state = !state;
    }, 1000);//Tempo entre chamadas => 1000 millis = 1 segundo 

    //Chamado quando a conexão com o client é fechada
    client.on('close', () => {
        console.log("Conexão fechada");
        //Remove o intervalo de envio de estado
        clearInterval(interval);
    });
});


function readData(){
    fs.readFile("level.json", "utf-8", (err, d) => {
        if (err) {
            console.log(err);
        }
        else {
            //console.log(data);
            var dado = JSON.parse(d);;
            data= dado;
            console.log(data);
        }
      });
}
function saveData(data){
    fs.writeFile("level.json", JSON.stringify(data), function(err) {
        if(err) {
            console.log(err);
        } else {
            console.log("The file was saved!");
        }
    })
}