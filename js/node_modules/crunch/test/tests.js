var assert = require('assert');
var Crunch = require('../');
var redis = require('redis');
var async = require('async');

describe('Crunch', function() {

	describe('#Crunch Relay functions', function() {
		it('should run relay functions when they are provided.', function(done) {
			var channel = 'testingCrunchRelay';
			var expectedMessage = 'expectedRelayedMessage';
			var relay = {};
			relay.subscribe = function () {
				//console.log('hook subscribed');
			};
			
			relay.unsubscribe = function () {
				//console.log('hook unsubscribed');
			};
			relay.message = function (message) {
				assert(message === expectedMessage);
				testClient.end();
				crunch.stop();
				assert(!crunch.started);
				done();
			};
			
			var crunch = Crunch.createInstance({ channel: channel, relay: relay });
			crunch.start();
			assert(crunch.started);
			
			var testClient = redis.createClient();
			
			testClient.publish(channel, expectedMessage);
		})
	})
})