const mqtt = require('mqtt');
const { resolve } = require('path');
const { getEnvironmentData } = require('worker_threads');

const fs = require("fs");

const options = {
    host: 'broker.hivemq.com',
    port: 1883

};


console.log("connecting");



client = mqtt.connect(options);

console.log("connected");
console.log("publishCam");
client.publish('espCam/img/get','1');
        
//client.subscribe('esp32/doorOpen');
console.log("subscribe","1");
client.on('message', function(topic, message){
    
    fs.writeFile('pic.jpeg', message, err => {
        if(err){
            console.error(err);
            return;
        }
    });

    console.log(`토픽:${topic.toString()}, 메시지: ${message.toString()}`)
    
    console.log("전달 완료");
    
})


