var mqtt = require('mqtt');
var config = require('./config');
var localBroker = config.localBroker;
var externalBroker = config.externalBroker;

var clientExternal = mqtt.connect(externalBroker.url, externalBroker.options);
var clientLocal = mqtt.connect(localBroker.url, localBroker.options);

clientExternal.on('connect', function() {
  console.log('Connected to external broker'); 
  externalBroker.topics.forEach(function(topic) {
    clientExternal.subscribe(topic, function(err) { 
      if (err) {
        console.error(err);
      }     
    }); 
  });
});

clientExternal.on('message', function(topic, message) {
  console.log('Received message from external:', topic, message.toString());
  clientLocal.publish(topic, message, {qos: 1});
});

clientLocal.on('connect', function() {
  console.log('Connected to local broker'); 
  localBroker.topics.forEach(function(topic) {
    clientLocal.subscribe(topic, function(err) {
      if (err) {
        console.error(err);
      }
    }); 
  });
});

clientLocal.on('message', function(topic, message) {
  console.log('Received message from local:', topic, message.toString());
  if (clientExternal.connected) {
    clientExternal.publish(topic, message, {qos: 1});
  } 
});
