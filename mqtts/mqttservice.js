/*
 * Copyright (c) 2020-2022 LG Electronics Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// helloworld_webos_service.js
// is simple service, based on low-level luna-bus API

// eslint-disable-next-line import/no-unresolved
const pkgInfo = require('./package.json');
const Service = require('webos-service');
const mqtt = require('mqtt');

const service = new Service(pkgInfo.name); // Create service by service name on package.json
const logHeader = "[" + pkgInfo.name + "]";
let greeting = "Hello, World!";

// a method that always returns the same value
service.register("getData", function(message) {
    console.log(logHeader, "SERVICE_METHOD_CALLED:/hello");
    console.log("In hello callback");
    const name = message.payload.name ? message.payload.name : "World";

  

    const options = {
        host: '',
        port: 1883

    };
    console.log("connecting");


    const client = mqtt.connect(options);


    client.subscribe('doorOpen');

    client.on('message', function(topic, msg){
        if(msg==1){
            console.log("닫힘");
          
        }
        else 
        console.log(`토픽:${topic.toString()}, 메시지: ${msg.toString()}`)

        message.respond({
            returnValue: true,
            Response: "Hello, " + "aaaaa" + "!"
        });
    });



});
