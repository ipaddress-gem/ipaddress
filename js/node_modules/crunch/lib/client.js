var Utils = require('./utils');
var Redis = require('redis');

var internals = {};


module.exports = internals.Client = function(options) {
	Utils.assert(this.constructor === internals.Client, 'Client must be instantiated using new');
	
	//options.channel is required
	
	this.client = Redis.createClient();
	
	//setup custom event relay
	
	this.message_count = 0;

	this.channel = options.channel;
	
	this.relay = options.relay;
	this.started = false;
};

internals.Client.prototype.start = function() {
	if (this.channel && !this.started) {
		var relay = this.relay;
		var msg_count = this.message_count;
		
		//add event listeners
		this.client.on('subscribe', function (channel, count) {
			//console.log("subscribe channel " + channel + ": " + count);
			if (relay && relay.subscribe) {
				relay.subscribe();
			}
		});

		this.client.on('unsubscribe', function (channel, count) {
			//console.log("unsubscribe channel " + channel + ": " + count);
			if (relay && relay.unsubscribe) {
				relay.unsubscribe();
			}
		});

		this.client.on('message', function (channel, message) {
			//console.log("client channel " + channel + ": " + message);
			msg_count++;
			if (relay && relay.message) {
				relay.message(message);
			}
			if (msg_count >= 3) {
				this.stop();
			}
		});
		
		//sub to redis channel
		this.client.subscribe(this.channel);
		this.started = true;
	}
};

internals.Client.prototype.stop = function() {
	if (this.started) {
		this.client.unsubscribe();
		
		//remove event listeners
		this.client.removeListener('subscribe', function (stream) {
			//console.log("removed subscribe listener");
		});

		this.client.removeListener('unsubscribe', function (stream) {
			//console.log("removed unsubscribe listener");
		});

		this.client.removeListener('message', function (stream) {
			//console.log("removed message listener");
		});
		
		this.client.end();
		this.started = false;
	}
};
