var mqtt = require('mqtt');
var config = require('./config');

var broker = config.broker;

var client = mqtt.connect(broker.url, broker.options);

client.on('connect', function() {
  console.log('Connected to external broker');
  client.subscribe(broker.topics.sequence, function(err, grants) {
    if (err) {
      console.error(err);
    } else {
      console.log(
        'Successfully subscribed to topic',
        broker.topics.sequence,
        'Received grants',
        grants
      );
    }
  });
});

client.on('message', function(topic, message) {
  console.log('Received message from topic:', topic, message.toString());
  client.publish(broker.topics.commands, message, {qos: 1}, function() {
    console.log('Published message to topic',
      broker.topics.commands, message);
  });
});
