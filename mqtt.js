const mqtt = require('mqtt');
const { resolve } = require('path');
const { getEnvironmentData } = require('worker_threads');

const options = {
    host: '34.82.211.143',
    port: 1883

};


console.log("connecting");



client = mqtt.connect(options);

console.log("connected");
client.subscribe('esp32/doorOpen');
console.log("subscribe");

client.on('message', function(topic, message){
    if(message==1){
        console.log("닫힘");
    }
    else console.log("열림");
    console.log(`토픽:${topic.toString()}, 메시지: ${message.toString()}`)
    
    console.log("전달 완료");
    
})


